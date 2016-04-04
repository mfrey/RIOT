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
 * @file        gateways.h
 * @brief       Maintains a list of active MQTT-SN gateways 
 *
 * @author      Michael Frey <mail@mfrey.net>
 */


#ifndef MQTTSN_GATEWAY_H_
#define MQTTSN_GATEWAY_H_


#include <stdint.h>
#include <stdbool.h>

#include "net/ipv6/addr.h"

#include "utlist.h"
#include "constants.h"

#ifdef __cpluslus
extern "C" {
#endif 

/**
 * A list element for a list of MQTT-SN gateways
 */
typedef struct mqttsn_gateway {
    /** the gateway id of the gateway */
    uint8_t gw_id;
    /** the address of the gateway */
    ipv6_addr_t *address;        
    /** the gateway interval of gateway */
    uint16_t duration;
    /** pointer to the next gateway in the list */
    struct mqttsn_gateway *next;
} mqttsn_gateway_entry_t;

void mqttsn_gateway_init(void);

bool mqttsn_gateway_add(uint8_t gw_id, ipv6_addr_t *address, uint16_t duration);

/**
 * Checks if the given topic id is a valid topic id 
 *
 * @param[in] topic_identifier The topic id to check
 *
 * @return True if the topic id is valid, false otherwise 
 */

/**
 * Checks if the given topic name is a valid topic name
 *
 * @param[in] topic_name The topic name to check
 *
 * @return True if the topic name is valid, false otherwise 
 */

/**
 * Checks if a topic with a given topic identifier is already 
 * registered.
 *
 * @param[in] topic_id The topic to check
 *
 * @return True if there is a topic registered with the given id, false
 * otherwise.
 */
bool mqttsn_gateway_contains(ipv6_addr_t *address);

// The correct return value?


/**
 * @brief Returns the number of active gateways.
 *
 * @return The number of active gateways.
 */
uint8_t mqttsn_gateway_size(void);

/**
 * @brief Removes all gateways from the list.
 */
void mqttsn_gateway_clear(void);

/**
 * Returns the most recently added gateway entry. The function is used by the 
 * mqttsn_handle_searchgw_msg() function. The specification leaves it open 
 * which gateway is returned to a client via the GWINFO packet (see also section
 * 6.1 of the MQTT-SN specification).
 *
 * @return Upon success the most recent added active gateway, NULL if there are
 * no active gateways.
 */
mqttsn_gateway_entry_t* mqttsn_gateway_get_most_recent_entry(void);

uint8_t mqttsn_gateway_compare(mqttsn_gateway_entry_t *first, mqttsn_gateway_entry_t *second);

#ifdef __cpluslus
}
#endif 

#endif /* MQTTSN_GATEWAY_H_ */
