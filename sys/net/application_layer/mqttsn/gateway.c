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

static mqttsn_gateway_entry_t *gateways = NULL;

void mqttsn_gateway_init(void) 
{

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

    LL_APPEND(gateways, &entry);

    return true;
}


mqttsn_gateway_entry_t* mqttsn_gateway_get_most_recent_entry(void) 
{
    return NULL;
}

/*
static int mqttsn_gateway_compare(mqttsn_gateway_entry_t *first, mqttsn_gateway_entry_t  *second) 
{
    return (ipv6_addr_equal((const ipv6_addr_t*)first->address, (const ipv6_addr-t*)second->address));
}
*/


bool mqttsn_gateway_contains(ipv6_addr_t *address) 
{
    mqttsn_gateway_entry_t *result = NULL;
    // TODO: temporary
    (void*)address;
    // TODO: wondering if this is going to work
 //   LL_SEARCH_SCALAR(gateways, result, address, mqttsn_gateway_compare);

    return (result != NULL);
}



uint8_t mqttsn_gateway_size(void)
{
    uint8_t size = 0;
    mqttsn_gateway_entry_t *entry;

    LL_COUNT(gateways, entry, size);

    return size;
}

void mqttsn_gateway_clear(void) 
{
    mqttsn_gateway_entry_t *entry, *tmp;

    LL_FOREACH_SAFE(gateways, entry, tmp) {
        LL_DELETE(gateways, entry);
    }
}
