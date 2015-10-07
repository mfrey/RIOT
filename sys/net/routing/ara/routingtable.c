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

#include <vtimer.h>

/***
 *
 */
static ara_routing_entry_t* ara_routing_table = NULL;

static timex_t null_time, last_access_time;

void ara_routing_table_init(void) 
{
    null_time = timex_set(1, 0);
    last_access_time = null_time;
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
        HASH_ADD_KEYPTR(hh, ara_routing_table, entry->destination, sizeof(struct netaddr), entry);
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

struct netaddr *ara_routing_table_get_next_hop(struct netaddr *destination)
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

bool ara_routing_table_is_deliverable(struct netaddr* destination)
{
    ara_routing_entry_t *entry = NULL;

    if((entry = ara_routing_table_get_entry(destination)) != NULL){
        /* there are actually next hops available */
        return entry->size > 0;
    }

    return false;
}


bool ara_routing_table_entry_exists(struct netaddr *destination) 
{
    return (ara_routing_table_get_entry(destination) != NULL);
}

ara_routing_entry_t* ara_routing_table_get_entry(struct netaddr *dest)
{
    ara_routing_entry_t *result = NULL;
    HASH_FIND(hh, ara_routing_table, dest, sizeof(struct netaddr), result);
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
        struct netaddr_str nbuf;
        ara_next_hop_t *element, *temporary_element;

        printf(".................................\n");
        printf("\t destination: %s\n", netaddr_to_string(&nbuf, entry->destination));

        DL_FOREACH_SAFE(entry->next_hops, element, temporary_element) {
            ara_print_next_hop_entry(element);

        }
    }
}

void ara_print_next_hop_entry(ara_next_hop_t *entry)
{
    struct netaddr_str nbuf;
    printf("\t\t next hop: %s\n", netaddr_to_string(&nbuf, entry->address));
    printf("\t\t phi: %f credit: %f ttl: %d\n", entry->phi, entry->credit, entry->ttl);
}

void ara_routing_table_add_next_hop(ara_routing_entry_t *entry, ara_next_hop_t *next_hop)
{
    ara_next_hop_t *result;
    DL_SEARCH(entry->next_hops, result, next_hop, ara_routing_table_next_hop_compare);

    if (!result) {
        DL_APPEND(entry->next_hops, next_hop);
        entry->size++;
    } else {
#if ENABLE_DEBUG
        struct netaddr_str buf;
        printf("there is already a next hop entry %s\n", netaddr_to_string(&buf, next_hop->address));
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

int ara_routing_table_next_hop_compare(ara_next_hop_t *first, ara_next_hop_t *second)
{
    return netaddr_cmp(first->address, second->address);
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

float ara_routing_table_get_pheromone_value(struct netaddr* destination, struct netaddr* next_hop)
{
    float result = 0.;
    ara_next_hop_t* entry = ara_routing_table_get_next_hop_entry(destination, next_hop);

    if (entry) {
        result = entry->phi;
    } else {
#if ENABLE_DEBUG
        struct netaddr_str buf;
        printf("there is no such next hop %s\n", netaddr_to_string(&buf, next_hop));
#endif
    }

    return result;
}

ara_next_hop_t* ara_routing_table_get_next_hop_entry(struct netaddr* destination, struct netaddr* next_hop)
{
    ara_next_hop_t *result = NULL;
    ara_routing_entry_t* entry = ara_routing_table_get_entry(destination);

    if (entry) {
//        DL_SEARCH(entry->next_hops, result, next_hop, ara_routing_table_next_hop_compare);
    } else {
#if ENABLE_DEBUG
        struct netaddr_str buf;
        printf("there is no such entry for destination address %s\n", netaddr_to_string(&buf, destination));
#endif
    }

    return result;
}

void ara_routing_table_update(struct netaddr* destination, struct netaddr* next_hop, void* interface, float new_pheromone_value)
{
    /* check if there is such an entry for the destination */
    ara_routing_entry_t* entry = ara_routing_table_get_entry(destination);

    /* there is already an entry for the destination */
    if (entry) {
        ara_next_hop_t *result = NULL;
        /* is the entry in the next hop list */
        DL_SEARCH(entry->next_hops, result, next_hop, ara_routing_table_next_hop_compare);

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
                
                HASH_ADD_KEYPTR(hh, ara_routing_table, entry->destination, sizeof(struct netaddr), entry);
            } else {
                /* the allocation of the next hop list failed */
                free(entry);
            }
        } else {
#if ENABLE_DEBUG
            struct netaddr_str buf;
            printf("could not create routing table entry for destination address %s\n", netaddr_to_string(&buf, destination));
#endif 
        }
    } 
}

ara_next_hop_t* ara_routing_table_create_next_hop(struct netaddr* address, float pheromone_value) {
    ara_next_hop_t* result = malloc(sizeof(ara_next_hop_t));

    if (result) {
        result->address = address;
        result->phi = pheromone_value;
    } else {
#if ENABLE_DEBUG
        struct netaddr_str buf;
        printf("could not create next hop entry for address %s\n", netaddr_to_string(&buf, address));
#endif 
    }

    return result;
}


void ara_routing_table_trigger_evaporation(void)
{
    timex_t current_time;
    /* get the current time */
    vtimer_now(&current_time);

    /* we access the routing table for the first time */
    if (timex_cmp(last_access_time, null_time)) {
        last_access_time = current_time;
    } else {
        ara_routing_table_apply_evaporation(current_time);
    }
}
    
void ara_routing_table_apply_evaporation(timex_t current_time)
{
    timex_t difference = timex_sub(current_time, last_access_time);

    // TODO
    //if (ara_time_within_interval(difference, ARA_EVAPORATION_INTERVAL)) {
    /* is it already time to update the entries */
    if (ara_time_within_interval(difference, 0)) {
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
                next_hop->phi = ara_evaporation_evaporate(next_hop->phi, 0);

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
