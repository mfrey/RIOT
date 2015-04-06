/*
 * Copyright (C) 2015 Freie Universität Berlin
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

static struct ara_routing_entry_t routing_table[ARA_MAX_ROUTING_ENTRIES];

void routingtable_init(void) 
{

    memset(&routing_table, 0, sizeof(routing_table));
}

struct netaddr *routingtable_get_next_hop(struct netaddr *destination)
{
    struct ara_routing_entry_t *entry = routingtable_get_entry(destination);

    if (!entry) {
        return NULL;
    }

    return NULL;
}

struct ara_routing_entry_t routingtable_get_entry(struct netaddr *destination)
{

    return NULL;
}

void print_routingtable(void)
{
    printf("===== BEGIN ROUTING TABLE ===================\n");
    for (int i = 0; i < ARA_MAX_ROUTING_ENTRIES; i++) {
         // add check if entries are up to date 
         print_routingtable_entry(&routing_table[i]);
    }
    printf("===== END ROUTING TABLE =====================\n");
}

void print_routingtable_entry(struct ara_routing_entry_t *entry)
{
    struct netaddr_str nbuf;
    printf(".................................\n");
    printf("\t destination: %s\n", netaddr_to_string(&nbuf, &(entry->destination)));
    for(int i = 0; i < entry->nextHopListSize; i++) {
         print_nexthop_entry(&entry->nextHops[i]);
    }
}

void print_nexthop_entry(struct ara_nexthop_entry_t *entry)
{
    struct netaddr_str nbuf;
    printf("\t\t next hop: %s\n", netaddr_to_string(&nbuf, &(entry->nextHop)));
    printf("\t\t phi: %f credit: %f ttl: %d\n", entry->phi, entry->credit, entry->ttl);
}
