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
 * @file        routingtable.h
 * @brief       ARA Routing Table
 *
 * @author      Michael Frey <frey@informatik.hu-berlin.de>
 */

#ifndef ARA_ROUTINGTABLE_H_
#define ARA_ROUTINGTABLE_H_

#include "uthash.h"
#include "utlist.h"
#include "constants.h"

#include <timex.h>
#include "common/netaddr.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * The ARA nexthop structure is used as an list element for the ARA routing table (i.e. a
 * destination might have multiple next hops to reach it)
 */                                                                                                                           
struct ara_next_hop_s 
{                                                                                                                                                     
    struct netaddr* address;     /**< the actual next hop */
    double phi;                  /**< pheromone value of the entry */
    double credit;               /**< how many times we may not receive an ack before the route is dropped */
    uint8_t ttl;                 /**< TTL for this entry */
    struct ara_next_hop_s* prev; /**< previous entry in the list */
    struct ara_next_hop_s* next; /**< next entry in the list */
};  

typedef struct ara_next_hop_s ara_next_hop_t;

/** 
 * ARA routing table entry 
 */
struct ara_routing_entry_s
{
    struct netaddr* destination; /**< destination address */
    timex_t lastAccessTime;      /**< last access time of the routing table entry */
    ara_next_hop_t* next_hops;   /**< list of all potential next hops */
    uint8_t size;                /**< size of the next hop list */
    UT_hash_handle hh;
};

typedef struct ara_routing_entry_s ara_routing_entry_t;

void ara_routing_table_add_entry(ara_routing_entry_t *entry);

/**
 * @brief     Returns a next hop for a given destination
 * @param[in] destination The destination address of a route
 */
struct netaddr *routingtable_get_next_hop(struct netaddr *destination);

/**
 * @brief     Deletes the routing table entry (if existent) for a given destination 
 * @param[in] destination The destination address of the entry which should be
 * removed 
 */
void routing_table_del_entry(struct netaddr *address);

/**
 * @brief     Return a routing table entry (if existent) for a given destination 
 * @param[in] destination The destination address of a route
 */
ara_routing_entry_t *routingtable_get_entry(struct netaddr *destination);

/**
 * @brief     Initializes the routing table.
 */
void ara_routing_table_init(void);

/**
 * @brief     Prints the routing table.
 */
void ara_print_routing_table(void);

/**
 * @brief     Prints a routing table entry.
 * @param[in] entry The routing table entry which should be printed
 */
void ara_print_routing_table_entry(ara_routing_entry_t *entry);

/**
 * @brief     Prints a next hop entry.
 * @param[in] entry The next hop entry which should be printed
 */
void ara_print_next_hop_entry(ara_next_hop_t *entry);

/**
 * @brief     Returns a next hop entry of a given entry at a given index
 * @param[in] entry The routing table entry 
 * @param[in] index The position of the next hop entry in the routing table entry
 */
ara_next_hop_t* ara_get_next_hop_entry(ara_routing_entry_t *entry, uint8_t index);

float ara_get_pheromone_value(ara_routing_entry_t *entry, uint8_t index);

/**
 * @brief     Adds a next hop entry to a given routing table entry
 * @param[in] entry The routing table entry for the next hop 
 * @param[in] next_hop The next hop entry to add
 */
void ara_add_next_hop_entry(ara_routing_entry_t *entry, ara_next_hop_t *next_hop);

bool ara_routing_table_entry_exists(struct netaddr *destination);

void ara_routing_table_del_next_hops(ara_routing_entry_t *entry);

int ara_next_hop_compare(ara_next_hop_t *first, ara_next_hop_t *second);

#ifdef __cplusplus
}
#endif

#endif /* ARA_ROUTINGTABLE_H_*/
