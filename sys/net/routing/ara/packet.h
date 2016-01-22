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

#ifndef ARA_PACKET_H__
#define ARA_PACKET_H_

#include "stdint.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum 
{
    DATA,  /**< data packet */
    FANT,  /**< forward ant agent */
    BANT   /**< backward ant agent */
} ara_packet_type; 

struct ara_packet_s 
{
    char type;                    /**< type of the packet */
    uint8_t seq_nr;               /**< sequence number */
    struct netaddr *source;       /**< source address of the packet address */
    struct netaddr *destination;  /**< destination address of the packet */
    uint8_t ttl;                  /**< time-to-live (ttl) of the packet */
};

typedef struct ara_packet_s ara_packet_t;

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

void ara_packets_make_forward_ant_agent();

void ara_packets_make_backward_ant_agent();

void ara_packets_make_clone();
*/

const char* ara_packet_get_type_as_string(char type);

ara_packet_t* ara_packets_make_route_failure_packet(struct netaddr *source, struct netaddr *destination, uint8_t sequence_number);

#ifdef __cplusplus
}
#endif

#endif /* ARA_PACKET_H_ */
