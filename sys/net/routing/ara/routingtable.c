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
 * @file        routing.c
 * @brief       ARA Routing Table.
 *
 * @author      Michael Frey <frey@informatik.hu-berlin.de>
 */

#include "ara.h"
#include "forwarding.h"
#include "evaporation.h"
#include "routingtable.h"

#include "xtimer.h"

#if ENABLE_DEBUG 
static char addr_str[IPV6_ADDR_MAX_STR_LEN];
#endif

/***
 *
 */
static ara_routing_entry_t* ara_routing_table = NULL;

float (*ara_routing_table_evaporate)(float old_pheromone_value, uint64_t milliseconds_since_last_evaporation);

uint64_t last_access_time;

void ara_routing_table_init(uint8_t evaporation_type) 
{
    last_access_time = 0;

    if (evaporation_type == 0) {
        ara_routing_table_evaporate = ara_evaporation_linear;
    /** 
     * if 1 or no valid number is passed to the function, we use by default a
     * exponential evaporation
     */
    } else {
        ara_routing_table_evaporate = ara_evaporation_exponential;
    }
}

void ara_routing_table_clear(void)
{
    ara_routing_entry_t *entry, *tmp; 

    HASH_ITER(hh, ara_routing_table, entry, tmp) {
        /* delete items from the next hop list */
        ara_routing_table_del_next_hops(entry);
        /* delete the actual entry */
        HASH_DEL(ara_routing_table, entry);
        /* free the entry */
        // free(entry);
    }
}

void ara_routing_table_add_entry(ara_routing_entry_t *entry)
{
    if (!ara_routing_table_entry_exists(entry->destination)) {
        HASH_ADD_KEYPTR(hh, ara_routing_table, entry->destination, sizeof(ipv6_addr_t), entry);
    }
}

void ara_routing_table_del_entry(ara_routing_entry_t *entry)
{
    if (entry) {
        /* stop the timer */

        /* reset the data */
        ara_routing_table_del_next_hops(entry);
        /* remove the entry from the hash table */
        HASH_DEL(ara_routing_table, entry);
        /* free the entry */
        //free(entry);
    }
}

ipv6_addr_t *ara_routing_table_get_next_hop(ipv6_addr_t *destination)
{
    ara_routing_entry_t *entry = ara_routing_table_get_entry(destination);

    if (!entry) {
        ara_next_hop_t *next_hop = ara_stochastic_forwarding(entry);

        if (!next_hop) {
            // TODO: better check about dangling pointers
            return next_hop->address;
        }
    }

    return NULL;
}

bool ara_routing_table_is_deliverable(ipv6_addr_t *destination)
{
    ara_routing_entry_t *entry = NULL;

    if((entry = ara_routing_table_get_entry(destination)) != NULL){
        /* there are actually next hops available */
        return entry->size > 0;
    }

    return false;
}


bool ara_routing_table_entry_exists(ipv6_addr_t *destination) 
{
    return (ara_routing_table_get_entry(destination) != NULL);
}

ara_routing_entry_t* ara_routing_table_get_entry(ipv6_addr_t *dest)
{
    ara_routing_entry_t *result = NULL;
    HASH_FIND(hh, ara_routing_table, dest, sizeof(ipv6_addr_t), result);
    return result;
}

void ara_print_routing_table(void)
{
    ara_routing_entry_t *entry;
    printf("===== BEGIN ROUTING TABLE ===================\n");
    for (entry = ara_routing_table; entry != NULL; entry = entry->hh.next) {
         // TODO: add check if entries are up to date 
         ara_print_routing_table_entry(entry);
    }
    printf("===== END ROUTING TABLE =====================\n");
}

void ara_print_routing_table_entry(ara_routing_entry_t *entry)
{
    if (entry->size > 0) {
        ara_next_hop_t *element, *temporary_element;

#if ENABLE_DEBUG
        if (ipv6_addr_to_str(addr_str, entry->destination, sizeof(addr_str)) != NULL) {
            printf(".................................\n");
            printf("\t destination: %s\n", addr_str);
        }
#endif

        DL_FOREACH_SAFE(entry->next_hops, element, temporary_element) {
            ara_print_next_hop_entry(element);

        }
    }
}

void ara_print_next_hop_entry(ara_next_hop_t *entry)
{
#if ENABLE_DEBUG
    if (ipv6_addr_to_str(addr_str, entry->address, sizeof(addr_str)) != NULL) {
        printf("\t\t next hop: %s\n", addr_str);
    }
#endif
    printf("\t\t phi: %f credit: %f ttl: %d\n", entry->phi, entry->credit, entry->ttl);
}

// TODO: add sanity check if entry does not exist, its created before the next
// hop is added 
void ara_routing_table_add_next_hop(ara_routing_entry_t *entry, ara_next_hop_t *next_hop)
{
    ara_next_hop_t *result;
    DL_SEARCH(entry->next_hops, result, next_hop, ara_routing_table_next_hop_compare);

    if (!result) {
        DL_APPEND(entry->next_hops, next_hop);
        entry->size++;
    } else {
#if ENABLE_DEBUG
        if (ipv6_addr_to_str(addr_str, next_hop->address, sizeof(addr_str)) != NULL) {
            printf("there is already a next hop entry %s\n", addr_str);
        }
#endif
    }
}

void ara_routing_table_del_next_hops(ara_routing_entry_t *entry) 
{
    if (entry->size > 0) {
        ara_next_hop_t *element, *temporary_element;

        DL_FOREACH_SAFE(entry->next_hops, element, temporary_element) {
            DL_DELETE(entry->next_hops, element); 
            entry->size--;
        }
    }
}

bool ara_routing_table_next_hop_compare(ara_next_hop_t *first, ara_next_hop_t *second)
{
    return ipv6_addr_equal(first->address, second->address);
}

// FIXME: might be a source of issues
ara_next_hop_t *ara_get_next_hop_entry(ara_routing_entry_t *entry, uint8_t position)
{
    uint8_t current_position = 0;
    ara_next_hop_t *result, *temporary_element;

    DL_FOREACH_SAFE(entry->next_hops, result, temporary_element) {
        if (current_position == position) {
            return result;
        }
        current_position++;
    }

    return NULL;
}

uint8_t ara_routing_table_size(void)
{
    return HASH_COUNT(ara_routing_table);
}

float ara_routing_table_get_pheromone_value(ipv6_addr_t* destination, ipv6_addr_t* next_hop)
{
    float result = 0.;
    ara_next_hop_t* entry = ara_routing_table_get_next_hop_entry(destination, next_hop);

    if (entry) {
        result = entry->phi;
    } else {
#if ENABLE_DEBUG
        if (ipv6_addr_to_str(addr_str, next_hop, sizeof(addr_str)) != NULL) {
            printf("there is no such next hop %s\n", addr_str);
        }
#endif
    }

    return result;
}

ara_next_hop_t* ara_routing_table_get_next_hop_entry(ipv6_addr_t* destination, ipv6_addr_t* next_hop)
{
    ara_next_hop_t *result = NULL;
    ara_routing_entry_t* entry = ara_routing_table_get_entry(destination);
    /**
     * since we can't search for addresses, but only next hop structures we have
     * to create one.
     */
    ara_next_hop_t next_hop_entry;
    next_hop_entry.address = next_hop;

    if (entry) {
        DL_SEARCH(entry->next_hops, result, &next_hop_entry, ara_routing_table_next_hop_compare);
    } else {
#if ENABLE_DEBUG
        if (ipv6_addr_to_str(addr_str, destination, sizeof(addr_str)) != NULL) {
            printf("there is no such entry for destination address %s\n", addr_str);
        }
#endif
    }

    return result;
}

void ara_routing_table_update(ipv6_addr_t* destination, ipv6_addr_t* next_hop, float new_pheromone_value)
{
    /* check if there is such an entry for the destination */
    ara_routing_entry_t* entry = ara_routing_table_get_entry(destination);

    /* there is already an entry for the destination */
    if (entry) {
        ara_next_hop_t *result = NULL;
        /**
         * since we can't search for addresses, but only next hop structures we have
         * to create one.
         */
        ara_next_hop_t next_hop_entry;
        next_hop_entry.address = next_hop;

        /* is the entry in the next hop list */
        DL_SEARCH(entry->next_hops, result, &next_hop_entry, ara_routing_table_next_hop_compare);

        if(result){
            result->phi = new_pheromone_value;
        /* create a new next hop entry */
        } else {
            /* create a new next hop */
            ara_next_hop_t* next_hop_entry = ara_routing_table_create_next_hop(next_hop, new_pheromone_value);

            if (next_hop_entry != NULL) {
                entry->next_hops = next_hop_entry;
            }
        }
    /* if there is no such entry */
    } else {
        entry = malloc(sizeof(ara_routing_entry_t));

        if (entry) { 
            /* create a new next hop */
            ara_next_hop_t* next_hop_entry = ara_routing_table_create_next_hop(next_hop, new_pheromone_value);

            /* TODO: check if parameters are an issue */
            if (next_hop) {
                /* set the destination */ 
                entry->destination = destination; 
                /* set the next hop list to the newly created element */
                entry->next_hops = next_hop_entry;
                
                HASH_ADD_KEYPTR(hh, ara_routing_table, entry->destination, sizeof(ipv6_addr_t), entry);
            } else {
                /* the allocation of the next hop list failed */
                free(entry);
            }
        } else {
#if ENABLE_DEBUG
            if (ipv6_addr_to_str(addr_str, destination, sizeof(addr_str)) != NULL) {
                printf("could not create routing table entry for destination address %s\n", addr_str);
            }
#endif 
        }
    } 
}

ara_next_hop_t* ara_routing_table_create_next_hop(ipv6_addr_t* address, float pheromone_value) {
    ara_next_hop_t* result = malloc(sizeof(ara_next_hop_t));

    if (result) {
        result->address = address;
        result->phi = pheromone_value;
    } else {
#if ENABLE_DEBUG
        if (ipv6_addr_to_str(addr_str, address, sizeof(addr_str)) != NULL) {
            printf("could not create next hop entry for address %s\n", address);
        }
#endif 
    }

    return result;
}


void ara_routing_table_trigger_evaporation(void)
{
    /* get the current time */
    uint64_t current_time = xtimer_now64();

    /* we access the routing table for the first time */
    if (last_access_time == 0) {
        last_access_time = current_time;
    } else {
        ara_routing_table_apply_evaporation(current_time);
    }
}

void ara_routing_table_apply_evaporation(uint64_t current_time)
{
    uint64_t difference = current_time - last_access_time;

    /* is it already time to update the entries */
    //if (difference >=  ARA_EVAPORATION_INTERVAL) {
    if (difference >=  10) {
        ara_routing_entry_t *entry, *tmp; 
        /**
         * for every destination we are going to update the pheromone values of
         * very next hop
         */
        HASH_ITER(hh, ara_routing_table, entry, tmp) {
            ara_next_hop_t *next_hop, *temporary_next_hop;

            /* update every next hop entry */
            DL_FOREACH_SAFE(entry->next_hops, next_hop, temporary_next_hop) {
                // TODO second parameter
                next_hop->phi = ara_routing_table_evaporate(next_hop->phi, 0);

                /* the route over this next hop evaporated */
                if (next_hop->phi == 0) {
                    /* erase the entry */
                    DL_DELETE(entry->next_hops, next_hop);
                }
            }

            int next_hop_list_size = 0;
            DL_COUNT(entry->next_hops, next_hop, next_hop_list_size);

            if (next_hop_list_size > 0) {
                /* erase the next hop list */
                DL_DELETE(entry->next_hops, next_hop);
                /* erase the entry in the routing table */
                HASH_DEL(ara_routing_table, entry);
            }
        }
    }
}
