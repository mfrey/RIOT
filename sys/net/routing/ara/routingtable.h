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

#include "constants.h"

#include <timex.h>
#include "common/netaddr.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * ARA routing table entry 
 */
struct ara_routing_entry_t
{
    struct netaddr destination;      /**< destination address */
    timex_t lastAccessTime;          /**< last access time of the routing table entry */
    struct ara_next_hop_t* nextHops; /**< list of all potential next hops */
    uint8_t nextHopListSize;         /**< the actual size of the next hop list */
};

/** 
 * The ARA nexthop structure is used as an list element for the ARA routing table (i.e. a
 * destination might have multiple next hops to reach it)
 */                                                                                                                           
struct ara_next_hop_t 
{                                                                                                                                                     
    struct netaddr nextHop;      /**< the actual next hop */
    double phi;                  /**< pheromone value of the entry */
    double credit;               /**< how many times we may not receive an ack before the route is dropped */
    uint8_t ttl;                 /**< TTL for this entry */
    struct ara_next_hop_t* prev; /**< previous entry in the list */
    struct ara_next_hop_t* next; /**< next entry in the list */
};     

/***
 *
 */
static struct ara_routing_entry_t routing_table[ARA_MAX_ROUTING_ENTRIES];

/**
 * @brief     Returns a next hop for a given destination
 * @param[in] destination The destination address of a route
 */
struct netaddr *routingtable_get_next_hop(struct netaddr *destination);

/**
 * @brief     Return a routing table entry (if existent) for a given destination 
 * @param[in] destination The destination address of a route
 */
struct ara_routing_entry_t *routingtable_get_entry(struct netaddr *destination);

/**
 * @brief     Initializes the routing table.
 */
void routingtable_init(void);

/**
 * @brief     Prints the routing table.
 */
void print_routing_table(void);

/**
 * @brief     Prints a routing table entry.
 * @param[in] entry The routing table entry which should be printed
 */
void print_routing_table_entry(struct ara_routing_entry_t *entry);

/**
 * @brief     Prints a next hop entry.
 * @param[in] entry The next hop entry which should be printed
 */
void print_next_hop_entry(struct ara_next_hop_t *entry);

#ifdef __cplusplus
}
#endif

#endif /* ARA_ROUTINGTABLE_H_*/
