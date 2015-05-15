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

#include "forwarding.h"
#include "routingtable.h"

/***
 *
 */
static ara_routing_entry_t* ara_routing_table = NULL;

void ara_routing_table_init(void) 
{
    // FIXME

    /*
    ara_routing_table = (ara_routing_entry_t*) malloc(sizeof(ara_routing_table_t));

    if (ara_routing_table) {
        ara_routing_table->table = (ara_routing_entry_t**) malloc(sizeof(ara_routing_entry_t*) * ARA_MAX_ROUTING_ENTRIES);

        if (ara_routing_table->table) {
            for (uint8_t i = 0; i < ARA_MAX_ROUTING_ENTRIES; i++) {
                ara_routing_table->table[i] = NULL;
            }

            ara_routing_table->size = ARA_MAX_ROUTING_ENTRIES;
        }
    }
    */
}

void ara_routing_table_add_entry(ara_routing_entry_t *entry)
{
    if (routingtable_get_entry(entry->destination)) {
        return;
    }

    // TODO
}

void routingtable_del_entry(struct netaddr address)
{
    /* find the routing table entry */
    ara_routing_entry_t *entry = routingtable_get_entry(&(address));

    if (entry != NULL) {
        /* stop the timer */

        /* reset the data  */

        /* remove the next hop list */
    }
}

struct netaddr *routingtable_get_next_hop(struct netaddr *destination)
{
    ara_routing_entry_t *entry = routingtable_get_entry(destination);

    if (!entry) {
        ara_next_hop_t *next_hop = ara_stochastic_forwarding(entry);

        if (!next_hop) {
            // TODO: better check about dangling pointers
            return next_hop->address;
        }

        return NULL;
    }

    return NULL;
}


ara_routing_entry_t* routingtable_get_entry(struct netaddr *destination)
{
    ara_routing_entry_t *result;
    HASH_FIND(hh, ara_routing_table, destination, sizeof(struct netaddr), result);
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
    struct netaddr_str nbuf;
    printf(".................................\n");
    printf("\t destination: %s\n", netaddr_to_string(&nbuf, entry->destination));
    for(int i = 0; i < entry->nextHopListSize; i++) {
         ara_print_next_hop_entry(&entry->nextHops[i]);
    }
}

void ara_print_next_hop_entry(ara_next_hop_t *entry)
{
    struct netaddr_str nbuf;
    printf("\t\t next hop: %s\n", netaddr_to_string(&nbuf, entry->address));
    printf("\t\t phi: %f credit: %f ttl: %d\n", entry->phi, entry->credit, entry->ttl);
}

ara_next_hop_t* ara_get_next_hop_entry(ara_routing_entry_t *entry, uint8_t index)
{
    /* check if the index is 'out of bounds' */
    if (entry->nextHopListSize > index) {
        ara_next_hop_t* ptr = entry->nextHops;

        for (uint8_t i = 0; i < index; i++) {
            ptr++;
        }

        return ptr;
    }

    return NULL;
}

float ara_get_pheromone_value(ara_routing_entry_t *entry, uint8_t index)
{
    ara_next_hop_t *next_hop = ara_get_next_hop_entry(entry, index);
    
    if (!next_hop){
        return next_hop->phi;
    }

    // TODO
    return -1.0;
}

void ara_add_next_hop_entry(struct netaddr *destination, ara_next_hop_t *entry)
{
    ara_routing_entry_t* routing_table_entry = routingtable_get_entry(destination);

    if (!routing_table_entry) {
        ara_next_hop_t* first = routing_table_entry->nextHops;

        if (!first) {
            ara_next_hop_t* last = routing_table_entry->nextHops->prev;

            /* there is only a single element in the list */
            if (last) {
                /* update the pointers of our formely single element */
                first->next = entry;
                first->prev = entry;
                /* update the pointers of the recently added element */
                entry->next = first;
                entry->prev = first;

            /* there is more than one element in the list */
            } else {
                /* the new element is the last element */
                last->next = entry;
                first->prev = entry;
                /* update the pointers of the recently added element */
                entry->prev = last;
                entry->next = first;
            }
        /* we are about to add the first element to the list */
        } else {
            routing_table_entry->nextHops = entry;
            entry->next = NULL;
            entry->prev = NULL;
        }

        // FIXME!!!
        /* update the size of the next hop list */
        //entry->nextHopListSize++;
    } else { 
#if ENABLE_DEBUG
        struct netaddr_str nbuf;
        printf("there is no routing table entry for %s\n", netaddr_to_string(&nbuf, destination));
#endif
    }
}
