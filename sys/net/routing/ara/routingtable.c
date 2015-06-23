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

bool ara_routing_table_entry_exists(struct netaddr *destination) 
{
    return (ara_routing_table_get_entry(destination) != NULL);
}

ara_routing_entry_t* ara_routing_table_get_entry(struct netaddr *dest)
{
    ara_routing_entry_t *result;
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
    DL_SEARCH(entry->next_hops, result, next_hop, ara_next_hop_compare);

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

int ara_next_hop_compare(ara_next_hop_t *first, ara_next_hop_t *second)
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
