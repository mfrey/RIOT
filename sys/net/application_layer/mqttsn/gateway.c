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
 * @file        gateway.c
 * @brief       Maintains a list of active MQTT-SN gateways 
 *
 * @author      Michael Frey <mail@mfrey.net>
 */

#include "gateway.h"

static uint8_t current_entry = 0;
static mqttsn_gateway_entry_t gateways[MQTTSN_DEFAULT_GATEWAY_SIZE];

void mqttsn_gateway_init(void) 
{
    memset(&gateways, 0, MQTTSN_DEFAULT_GATEWAY_SIZE);
}


bool mqttsn_gateway_add(uint8_t gw_id, ipv6_addr_t *address, uint16_t duration) 
{

    if (mqttsn_gateway_contains(address)) {
#if ENABLE_DEBUG 
        printf("there is already a gateway registered with this ipv6 address!\n");
#endif 
        return false;
    }
    
    mqttsn_gateway_entry_t entry;
    entry.gw_id = gw_id;
    entry.address = *address;
    entry.duration = duration;

// TODO: howto manage last access time 
    gateways[current_entry] = entry;

    current_entry++;

    return true;
}


mqttsn_gateway_entry_t* mqttsn_gateway_get_most_recent_entry(void) 
{
    return &(gateways[current_entry]);
}

bool mqttsn_gateway_contains(ipv6_addr_t *address) 
{
    for (int i = 0; i <= current_entry; i++) {
        mqttsn_gateway_entry_t entry = gateways[i];

        if (ipv6_addr_equal(&(entry.address), address)) {
            return true;
        }
    }

    return false;
}



uint8_t mqttsn_gateway_size(void)
{
    return current_entry;
}

void mqttsn_gateway_clear(void) 
{
    memset(&gateways, 0, MQTTSN_DEFAULT_GATEWAY_SIZE);
    current_entry = 0;
}
