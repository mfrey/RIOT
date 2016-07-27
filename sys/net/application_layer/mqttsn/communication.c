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
 * @brief       Functions to send and receive MQTT-SN messages
 *
 * @author      Michael Frey <mail@mfrey.net>
 */

#include "communication.h" 
#include "messages.h"
#include "constants.h"
#include "device.h"

#include "net/gnrc.h"
#include "net/gnrc/ipv6.h"
#include "net/gnrc/nettype.h"
#include "net/gnrc/udp.h"
#include "net/gnrc/pktdump.h"

#include <string.h>

#define ENABLE_DEBUG 1

#ifdef __cplusplus
extern "C" {
#endif

/** pid for the mqtt-sn process  */
kernel_pid_t mqttsn_receiver_pid = KERNEL_PID_UNDEF;
/** message queue for the mqtt-sn process */
static char _mqttsn_receive_stack[MQTTSN_STACK_SIZE];
/** */
static bool forward_encapsulation = false;
/** */
static ipv6_addr_t source;
/** */
static uint16_t source_port;
/** */
static ipv6_addr_t destination;
/** */
static uint16_t destination_port;
/** */
static uint8_t wireless_node_id[MQTTSN_MAX_WIRELESS_NODE_ID_LENGTH];  
/** */
static uint8_t wireless_node_length;
/***/
static void* mqttsn_communication_receive_udp_event_loop(void *args);
/***/
static bool mqttsn_communication_is_udp_packet(char *data);

void mqttsn_communication_init(ipv6_addr_t src, uint16_t src_port, bool enable_forward_encapsulation)
{
#if ENABLE_DEBUG 
    printf("initialize communication module\n");
#endif 
    source = src;
    //destination = dest;
    source_port = src_port;
    //destination_port = dest_port;

    forward_encapsulation = enable_forward_encapsulation;

    /** TODO: make this a parameter */
    mqttsn_device_init(NULL, 0);
    /** set the size */
    wireless_node_length = mqttsn_device_get_node_length();
    /** set the wireless node id */
    memcpy(wireless_node_id, mqttsn_device_get_node(), wireless_node_length);
}

bool mqttsn_communication_is_broadcast_message(uint8_t type)
{
    return ((type == MQTTSN_TYPE_SEARCHGW) || (type == MQTTSN_TYPE_GWINFO) || (type == MQTTSN_TYPE_ADVERTISE));
}

void mqttsn_communication_set_destination(ipv6_addr_t *dest)
{
    destination = *dest;
}

void mqttsn_communication_set_destination_port(uint16_t port)
{
    destination_port = port;
}

void mqttsn_communication_send_udp(void *packet)
{
    /** the first octet always specifies the length of the packet */
    size_t length = ((uint8_t*)packet)[0];
    /** retrieve the type of the packet */
    uint8_t type = mqttsn_get_type(packet);
    /** is the message a "broadcast" message */
    bool is_broadcast_message = mqttsn_communication_is_broadcast_message(type);

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
    if (is_broadcast_message) { 
        /** determine the site global address we might use for "broadcast" */
        ipv6_addr_t *broadcast_address = NULL;  // TODO
        ip = gnrc_ipv6_hdr_build(udp, NULL, broadcast_address);
    } else {
        ip = gnrc_ipv6_hdr_build(udp, NULL, &destination);
    }

    if (!ip) {
#if ENABLE_DEBUG 
        printf("could not add ip header to packet buffer\n");
#endif 
        gnrc_pktbuf_release(udp);
        return;
    }

    /**
     * The 'broadcast radius' (see section 6.1 of the MQTT-SN specification)
     * indicates to how many hops a packet is forwarded.
     */
    if (is_broadcast_message) { 
        // TODO: forward encapsulation? what to do?
        ((ipv6_hdr_t*)ip->data)->hl = mqttsn_get_radius();
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

void mqttsn_communication_receive_udp(void)
{
    /* initialize the mqtt-sn thread */
    if (mqttsn_receiver_pid == KERNEL_PID_UNDEF) {
        mqttsn_receiver_pid = thread_create(_mqttsn_receive_stack, sizeof(_mqttsn_receive_stack), MQTTSN_PRIO,
                THREAD_CREATE_STACKTEST, mqttsn_communication_receive_udp_event_loop, NULL, "mqtt-sn receiver");
    }
}

static void* mqttsn_communication_receive_udp_event_loop(void *args)
{
    msg_t message; 
    msg_t message_queue[MQTTSN_MSG_QUEUE_SIZE];
    gnrc_netreg_entry_t registration;

    /** avoid the warning about the unused variable*/
    (void)args;

    registration.demux_ctx = GNRC_NETREG_DEMUX_CTX_ALL;
    registration.pid = thread_getpid();
    /** register interest in all udp packets */
    gnrc_netreg_register(GNRC_NETTYPE_UDP, &registration);


    if(msg_init_queue(message_queue, MQTTSN_MSG_QUEUE_SIZE) == -1){
#if ENABLE_DEBUG 
        printf("could not initialize message queue!\n");
#endif 
        return NULL;
    }

    while (1) {
        msg_receive(&message);

        switch (message.type) {
            case GNRC_NETAPI_MSG_TYPE_RCV:
                /** is the received packet a udp packet */
                if (mqttsn_communication_is_udp_packet(message.content.ptr)) {
                    ipv6_addr_t *source = NULL;
                    /** pass the payload of the udp packet to mqtt-sn */
                    mqttsn_handle_msg(message.content.ptr, source);
                }
                break;
            default:
                break;
        }
    }

    return NULL;
}

static bool mqttsn_communication_is_udp_packet(char *data) 
{
    gnrc_pktsnip_t *packet = (gnrc_pktsnip_t *)data;
    return (packet->type == GNRC_NETTYPE_UDP);
}

bool mqttsn_communication_is_forwarder_encapsulation_enabled(void) 
{
    return forward_encapsulation;
}


#ifdef __cplusplus
}
#endif
