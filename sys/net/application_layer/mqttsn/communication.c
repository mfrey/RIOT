/*
 * Copyright (C) 2016 Michael Frey <mail@mfrey.net>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     mqtt-sn
 * @{
 *
 * @file        communication.c
 * @brief       ARA routing protocol
 *
 * @author      Michael Frey <mail@mfrey.net>
 */

#include "communication.h" 
#include "mqttsn.h"
#include "constants.h"

#include "net/gnrc.h"
#include "net/gnrc/ipv6.h"
#include "net/gnrc/udp.h"
#include "net/gnrc/pktdump.h"

#ifdef __cplusplus
extern "C" {
#endif

static bool forward_encapsulation = false;
static ipv6_addr_t source;
static uint16_t source_port;
static ipv6_addr_t destination;
static uint16_t destination_port;
uint8_t wireless_node_id[MQTTSN_MAX_WIRELESS_NODE_ID_LENGTH];  
uint8_t wireless_node_length;

void mqttsn_communication_init(ipv6_addr_t src, uint16_t src_port, ipv6_addr_t dest, uint16_t dest_port, bool enable_forward_encapsulation)
{
    source = src;
    destination = dest;
    source_port = src_port;
    destination_port = dest_port;

    forward_encapsulation = enable_forward_encapsulation;
}

void mqttsn_communication_send_udp(void *packet)
{
    /** the first octet always specifies the length of the packet */
    size_t length = ((uint8_t*)packet)[0];

    gnrc_pktsnip_t *payload, *udp, *ip;

    // TODO: check if it makes sense to create a nettype for MQTT-SN
    payload = gnrc_pktbuf_add(NULL, packet, length, GNRC_NETTYPE_UNDEF);
    if (!payload) {
#if ENABLE_DEBUG 
        printf("could not add payload to packet buffer\n");
#endif 
        return;
    }

    /** check if forward encapsulation is enabled */
    if (forward_encapsulation) {
        /** the packet needs to be wrapped according to the specification */
        mqttsn_msg_forward_encapsulation_t forward_encapsulation_packet;
        /** set the wireless node id */
        memcpy(&(forward_encapsulation_packet.wireless_node_id), wireless_node_id, wireless_node_length);
        /** set the packet type */
        forward_encapsulation_packet.header.msg_type = MQTTSN_TYPE_ENCMSG;
        // TODO
        forward_encapsulation_packet.ctrl = 0;
        /** set the packet length of the forward encapsulation packet */
        forward_encapsulation_packet.header.length = wireless_node_length + 3;

        /** append forward encapsulation packet to */
        gnrc_pktsnip_t *forward_encapsulation_payload = gnrc_pktbuf_add(payload, (void*)&forward_encapsulation_packet, 
                forward_encapsulation_packet.header.length, GNRC_NETTYPE_UNDEF);
        /** */
        if (!forward_encapsulation_payload) {
#if ENABLE_DEBUG 
            printf("could not add forward encapsulation payload to packet buffer\n");
#endif 
            gnrc_pktbuf_release(payload);
            return;
        }

        /** build udp header */
        udp = gnrc_udp_hdr_build(forward_encapsulation_payload, source_port, destination_port);
    } else {
        /** build udp header */
        udp = gnrc_udp_hdr_build(payload, source_port, destination_port);
    }

    if (!udp) {
#if ENABLE_DEBUG 
        printf("could not add udp header to packet buffer\n");
#endif 
        gnrc_pktbuf_release(payload);
        return;
    }

    /** build ip header */
    ip = gnrc_ipv6_hdr_build(udp, NULL, &destination);
    if (!ip) {
#if ENABLE_DEBUG 
        printf("could not add ip header to packet buffer\n");
#endif 
        gnrc_pktbuf_release(udp);
        return;
    }

    /** send packet */
    if (!gnrc_netapi_dispatch_send(GNRC_NETTYPE_UDP, GNRC_NETREG_DEMUX_CTX_ALL, ip)) {
#if ENABLE_DEBUG 
        printf("Error: unable to locate UDP thread\n");
#endif 
        gnrc_pktbuf_release(ip);
        return;
    }
}

#ifdef __cplusplus
}
#endif
