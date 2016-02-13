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
 * @file        packet.h
 * @brief       Definitions of ARA packets and functions to create ARA packets 
 *
 * @author      Michael Frey <frey@informatik.hu-berlin.de>
 */

#ifndef ARA_PACKET_H_
#define ARA_PACKET_H_

#include "stdint.h"
#include "stdbool.h"

#include "net/gnrc/nettype.h"
#include "net/ipv6/addr.h"
#include "net/ipv6/hdr.h"
#include "net/gnrc.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum 
{
    DATA,         /**< data packet */
    FANT,         /**< forward ant agent */
    BANT,         /**< backward ant agent */
    ROUTE_FAILURE /**< route failure */
} ara_packet_type; 

inline bool ara_packet_is_ant_packet(char type)
{
    switch (type) {
        case FANT: 
        case BANT:
            return true;
        default: 
            return false; 
    }
}

/*
void ara_packets_initialize(uint8_t max_hop_count);
*/

char* ara_packet_prepare_payload(char type, char* payload, uint8_t payload_size);

gnrc_pktsnip_t* ara_packet_make_bant(gnrc_pktsnip_t* packet);

gnrc_pktsnip_t* ara_packet_make_fant(ipv6_addr_t* source, ipv6_addr_t* destination);

gnrc_pktsnip_t* ara_packet_make_route_failure(ipv6_addr_t* source, ipv6_addr_t* destination);

gnrc_pktsnip_t* ara_packet_make_packet(ipv6_addr_t* source, ipv6_addr_t* destination, ipv6_addr_t* sender, 
        char type, char* data, uint8_t data_size, ipv6_addr_t* previous_hop);

const char* ara_packet_get_type_as_string(char type);


#ifdef __cplusplus
}
#endif

#endif /* ARA_PACKET_H_ */
