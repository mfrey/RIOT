/*
 * Copyright (C) 2015 Michael Frey <frey@informatik.hu-berlin.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     ara
 * @{
 *
 * @file        packet.c
 * @brief       ARA packets and helper functions.
 *
 * @author      Michael Frey <frey@informatik.hu-berlin.de>
 */

#include "packet.h" 

const char* ara_packet_get_type_as_string(char type)
{
    switch (type) {
        case DATA: 
            return "DATA\0";
        case FANT: 
            return "FANT\0";
        case BANT: 
            return "BANT\0";
        case ROUTE_FAILURE: 
            return "ROUTE FAILURE\0";
        default: 
            return "UNKNOWN\0";
    }
}

gnrc_pktsnip_t* ara_packet_make_fant(ipv6_addr_t* source, ipv6_addr_t* destination) 
{
    return ara_packet_make_packet(source, destination, source, FANT, NULL, 0, NULL);
}

gnrc_pktsnip_t* ara_packet_make_route_failure(ipv6_addr_t* source, ipv6_addr_t* destination) 
{
    return ara_packet_make_packet(source, destination, source, ROUTE_FAILURE, NULL, 0, NULL);
}

gnrc_pktsnip_t* ara_packet_make_bant(gnrc_pktsnip_t* packet) 
{
    /** check if the packet is an ipv6 packet */
    if (packet->type != GNRC_NETTYPE_IPV6) {
        /** TODO: print debug message */
        return NULL;
    }

    ipv6_hdr_t* header = (ipv6_hdr_t*)packet->data;
    /** determine original source of the packet  */
    ipv6_addr_t *source = &(header->src);
    /** determine original destination of the packet  */
    ipv6_addr_t *destination = &(header->dst);
    /** build backward ant agent */
    return ara_packet_make_packet(destination, source, destination, BANT, NULL, 0, NULL);
}

gnrc_pktsnip_t* ara_packet_make_packet(ipv6_addr_t* source, ipv6_addr_t* destination, ipv6_addr_t* sender, 
        char type, char* data, uint8_t data_size, ipv6_addr_t* previous_hop) 
{
    gnrc_pktsnip_t *packet, *payload;

    payload = gnrc_pktbuf_add(NULL, data, data_size, GNRC_NETTYPE_UNDEF);

    /** allocation of payload failed */
    if (payload == NULL) {
        return NULL;
    }

    packet = gnrc_ipv6_hdr_build(payload, (uint8_t*)source, sizeof(ipv6_addr_t), (uint8_t*)destination, sizeof(ipv6_addr_t));

    /** allocation of ipv6 header failed */
    if (packet == NULL) {
        gnrc_pktbuf_release(payload);
        return NULL;
    }

    return packet;
}

char* ara_packet_prepare_payload(char type, char* payload, uint8_t payload_size)
{
    return NULL;
}
