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
    //
}

void ara_routing_table_add_entry(ara_routing_entry_t *entry)
{
    if (!ara_routing_table_entry_exists(entry->destination)) {
        HASH_ADD_KEYPTR(hh, ara_routing_table, entry->destination, sizeof(struct netaddr), entry);
    }
}

void routingtable_del_entry(struct netaddr address)
{
    /* find the routing table entry */
    ara_routing_entry_t *entry = routingtable_get_entry(&(address));

    if (entry) {
        /* stop the timer */

        /* remove the entry from the hash table */
        HASH_DEL(ara_routing_table, entry);
        /* reset the data */
        ara_routing_table_del_next_hops(entry);
        /* free the entry */
        free(entry);
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
    }

    return NULL;
}

bool ara_routing_table_entry_exists(struct netaddr *destination) 
{
    return (routingtable_get_entry(destination) != NULL);
}

ara_routing_entry_t* routingtable_get_entry(struct netaddr *destination)
{
    ara_routing_entry_t *result = NULL;
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

void ara_routing_table_del_next_hops(ara_routing_entry_t *entry)
{
    /* there are no entries in the next hop list */
    if (entry->nextHopListSize == 0) {
        return;
    /* there is only a single entry in the next hop list */
    } else if (entry->nextHopListSize == 1) {
        free(entry->nextHops);
        return;
    } else {
        // TODO: check if that makes sense
        // FIXME: that doesn't make sense, also think about if you want to add actually make it a ring 
        for (uint8_t i = 0; i < entry->nextHopListSize; i++) {
            ara_next_hop_t *next_hop = entry->nextHops;

            if (next_hop) {
                /* update the next/prev ptrs of the previous/next next hop */
                next_hop->prev->next = next_hop->next;
                next_hop->next->prev = next_hop->prev;
                /* update the start of the next hop list */
                entry->nextHops = next_hop->next;
                /* set current next/prev to null */
                next_hop->next = NULL;
                next_hop->prev = NULL;

                /* free the entry */
                free(next_hop);
            }
        }
    }
}
