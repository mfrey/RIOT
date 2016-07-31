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
#include "stdlib.h"

static mqttsn_gateway_entry_t *gateways = NULL;

int8_t mqttsn_gateway_cmp(mqttsn_gateway_entry_t *first, mqttsn_gateway_entry_t *second) 
{
    /**
     * The function is used for the DL_SEARCH macro of utlist. The list requires
     * a compare function which behaves like strmcmp in terms of return types.
     * Particularly, the function has to return negative, zero, or positive value, 
     * which specifies whether the first item should sort before, equal to, or after 
     * the second item, respectively.
     *
     * However, we ignore the sorting. Please, be aware if you intend to use
     * that function for sorting purposes in utlist.
     */
    if (!ipv6_addr_equal(&(first->address), &(second->address))) {
        return -1;
    }

    // TODO: think about gw_id

    return 0;
}

bool mqttsn_gateway_add(uint8_t gw_id, ipv6_addr_t *address, uint16_t duration) 
{
    if (!mqttsn_gateway_contains(address)) {
        mqttsn_gateway_entry_t *entry = (mqttsn_gateway_entry_t*)malloc(sizeof(mqttsn_gateway_entry_t));

        if (entry != NULL) {
            /** set gateway id */
            entry->gw_id = gw_id;
            /** set address */
            memcpy(&(entry->address), address, sizeof(ipv6_addr_t));
            // TODO
            entry->duration = duration;
            /** add the newly created entry to the list */
            DL_APPEND(gateways, entry);

            return true;
        }
    } else {
#if ENABLE_DEBUG 
        printf("there is already a gateway registered with this ipv6 address!\n");
#endif 
    }
    
    return false;
}

mqttsn_gateway_entry_t* mqttsn_gateway_get_most_recent_entry(void) 
{
    if (gateways != NULL) {
        return gateways->prev;
    }

    return NULL;
}

bool mqttsn_gateway_contains(ipv6_addr_t *address) 
{
    mqttsn_gateway_entry_t *entry, tmp;
    // TODO: check!
    tmp.address = *address;
    DL_SEARCH(gateways, entry, &tmp, mqttsn_gateway_cmp);
    return (entry != NULL);
}

uint8_t mqttsn_gateway_size(void)
{
    uint8_t result = 0;
    mqttsn_gateway_entry_t *entry;
    DL_COUNT(gateways, entry, result);
    return result;
}

void mqttsn_gateway_clear(void) 
{
    mqttsn_gateway_entry_t *entry, *tmp;

    DL_FOREACH_SAFE(gateways, entry, tmp) {
        DL_DELETE(gateways, entry);
        free(entry);
    }
}

void mqttsn_gateway_print(uint8_t gw_id)
{
    mqttsn_gateway_entry_t *gateway;
    char address[IPV6_ADDR_MAX_STR_LEN];

    DL_FOREACH(gateways, gateway) {
        ipv6_addr_to_str(address, &(gateway->address), IPV6_ADDR_MAX_STR_LEN);

        /** checks if the current list element is the active gateway */
        if (gateway->gw_id == gw_id) { 
            /** a '*' indicates the active gateway */
            printf("[*] gateway id: %d, address: %s, duration: %d\n", gateway->gw_id, address, gateway->duration);
        } else {
            printf("[ ] gateway id: %d, address: %s, duration: %d\n", gateway->gw_id, address, gateway->duration);
        }
    }
}
