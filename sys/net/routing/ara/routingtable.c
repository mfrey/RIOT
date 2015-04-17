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

#include "routingtable.h"


void routingtable_init(void) 
{
    /* initialize routing table */
    memset(&routing_table, 0, sizeof(routing_table));
}

void routingtable_add_entry(struct ara_routing_entry_t *entry)
{
    /* only add an entry if there is not an entry with the specified addres */ 
    if (routingtable_get_entry(&(entry->destination))) {
        return;
    }

    /* find a free spot in the routing table */
    for (unsigned i = 0; i < ARA_MAX_ROUTING_ENTRIES; i++) {
        // TODO: check if this really works 
        if (routing_table[i].destination == 0) {
            memcpy(&routing_table[i], entry, sizeof(struct ara_routing_entry_t));
            return;
        }
    }
}

struct netaddr *routingtable_get_next_hop(struct netaddr *destination)
{
    struct ara_routing_entry_t *entry = routingtable_get_entry(destination);

    if (!entry) {
        return NULL;
    }

    return NULL;
}

struct ara_routing_entry_t* routingtable_get_entry(struct netaddr *destination)
{
    for (unsigned i = 0; i < ARA_MAX_ROUTING_ENTRIES; i++) {
        if (!netaddr_cmp(&routing_table[i].destination, destination)) {
#if ENABLE_DEBUG
            print_routing_table_entry(&routing_table[i]);
#endif
            return &routing_table[i];
        }
    }

    return NULL;
}

void print_routing_table(void)
{
    printf("===== BEGIN ROUTING TABLE ===================\n");
    for (int i = 0; i < ARA_MAX_ROUTING_ENTRIES; i++) {
         // add check if entries are up to date 
         print_routing_table_entry(&routing_table[i]);
    }
    printf("===== END ROUTING TABLE =====================\n");
}

void print_routing_table_entry(struct ara_routing_entry_t *entry)
{
    struct netaddr_str nbuf;
    printf(".................................\n");
    printf("\t destination: %s\n", netaddr_to_string(&nbuf, &(entry->destination)));
    for(int i = 0; i < entry->nextHopListSize; i++) {
         print_next_hop_entry(&entry->nextHops[i]);
    }
}

void print_next_hop_entry(struct ara_next_hop_t *entry)
{
    struct netaddr_str nbuf;
    printf("\t\t next hop: %s\n", netaddr_to_string(&nbuf, &(entry->nextHop)));
    printf("\t\t phi: %f credit: %f ttl: %d\n", entry->phi, entry->credit, entry->ttl);
}
