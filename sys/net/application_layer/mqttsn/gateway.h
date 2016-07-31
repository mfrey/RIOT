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
    ipv6_addr_t address;        
    /** the gateway interval of gateway */
    uint16_t duration;
    /** pointers to the next/previous gateway element */
    struct mqttsn_gateway *next, *prev;
} mqttsn_gateway_entry_t;

void mqttsn_gateway_init(void);

bool mqttsn_gateway_add(uint8_t gw_id, ipv6_addr_t *address, uint16_t duration);

/**
 * @brief Checks if a gateway with a given address is already 
 * registered.
 *
 * @param[in] address The address of the gateway
 *
 * @return True if there is a gateway registered with the given address, false
 * otherwise.
 */
bool mqttsn_gateway_contains(ipv6_addr_t *address);

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

/**
 * The function checks if the addresses of the two given gateway entries are equal 
 * or not. This* function is used for utlist's xL_SEARCH function. However, the
 * function only returns 0 or -1 if addresses match or not.
 *
 * @param[in] first The gateway which is compared to the second gateway
 * @param[out] second The gateway which is compared to the first gateway
 *
 * @return 0 if addresses of the gateway entries match, -1 otherwise.
 */
int8_t mqttsn_gateway_compare(mqttsn_gateway_entry_t *first, mqttsn_gateway_entry_t *second);

/**
 * @brief Prints all known gateways to the command line.
 *
 * @param[in] gw_id The active gatewy id.
 */
void mqttsn_gateway_print(uint8_t gw_id);

#ifdef __cpluslus
}
#endif 

#endif /* MQTTSN_GATEWAY_H_ */
