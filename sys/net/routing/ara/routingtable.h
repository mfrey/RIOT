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

#include "net/ipv6/addr.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * The ARA nexthop structure is used as an list element for the ARA routing table (i.e. a
 * destination might have multiple next hops to reach it)
 */                                                                                                                           
struct ara_next_hop_s 
{                                                                                                                                                     
    ipv6_addr_t* address;     /**< the actual next hop */
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
    ipv6_addr_t* destination; /**< destination address */
    uint64_t last_access_time;   /**< last access time of the routing table entry */
    ara_next_hop_t* next_hops;   /**< list of all potential next hops */
    uint8_t size;
    UT_hash_handle hh;
};

typedef struct ara_routing_entry_s ara_routing_entry_t;

/**
 * @brief     Adds a routing entry to the routing table
 * @param[in] entry The routing table entry which is going to be added
 */
void ara_routing_table_add_entry(ara_routing_entry_t *entry);

/**
 * @brief     Returns a next hop for a given destination
 * @param[in] destination The destination address of a route
 */
ipv6_addr_t *ara_routing_table_get_next_hop(ipv6_addr_t *destination);

/**
 * @brief     Deletes the routing table entry (if existent) for a given destination 
 * @param[in] destination The destination address of the entry which should be
 * removed 
 */
void ara_routing_table_del_entry(ara_routing_entry_t *entry);

/**
 * @brief     Return a routing table entry (if existent) for a given destination 
 * @param[in] destination The destination address of a route
 */
ara_routing_entry_t *ara_routing_table_get_entry(ipv6_addr_t *destination);

/**
 * @brief     Initializes the routing table.
 * @param[in] The type of the evaporation function (0 = linear, 1 = exponential)
 */
void ara_routing_table_init(uint8_t type);

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

/**
 * @brief     Adds a next hop entry to a given routing table entry
 * @param[in] entry The routing table entry for the next hop 
 * @param[in] next_hop The next hop entry to add
 */
void ara_routing_table_add_next_hop(ara_routing_entry_t *entry, ara_next_hop_t *next_hop);

/**
 * @brief     Checks if a routing table entry for a given destination exists
 * @param[in] destination The destination address of the routing table entry 
 */
bool ara_routing_table_entry_exists(ipv6_addr_t *destination);

/**
 * @brief     Returns the size of the routing table
 */
uint8_t ara_routing_table_size(void);

/**
 * @brief     Removes the next hops of a routing table entry 
 * @param[inout] entry The routing table entry which next hops are going to be
 * removed
 */
void ara_routing_table_del_next_hops(ara_routing_entry_t *entry);

/**
 * @brief     Checks if a destination has next hops 
 * @param[in] destination The destination address which is going to be checked
 * @return    True if a destination has next hops and hence a packet can
 * be delivered, otherweise false
 */
bool ara_routing_table_is_deliverable(ipv6_addr_t* destination);

// TODO
ara_next_hop_t *ara_get_next_hop_entry(ara_routing_entry_t *entry, uint8_t position);

bool ara_routing_table_next_hop_compare(ara_next_hop_t *first, ara_next_hop_t *second);

void ara_routing_table_clear(void);

float ara_routing_table_get_pheromone_value(ipv6_addr_t* destination, ipv6_addr_t* next_hop);
// TODO
ara_next_hop_t* ara_routing_table_get_next_hop_entry(ipv6_addr_t* destination, ipv6_addr_t* next_hop);

void ara_routing_table_update(ipv6_addr_t* destination, ipv6_addr_t* next_hop, float new_pheromone_value);

/**
 * @brief The function triggers the evaporation process, iff enough time has
 * passed
 */
void ara_routing_table_trigger_evaporation(void);

/**
 * @brief The function updates for every destination the pheromone values of the
 * next hop list
 * @param[in] timestamp The time at which the evaporation process was triggered
 */
void ara_routing_table_apply_evaporation(uint64_t timestamp);

ara_next_hop_t* ara_routing_table_create_next_hop(ipv6_addr_t* address, float pheromone_value);

extern float (*ara_routing_table_evaporate)(float old_pheromone_value, uint64_t milliseconds_since_last_evaporation);

#ifdef __cplusplus
}
#endif

#endif /* ARA_ROUTINGTABLE_H_*/
