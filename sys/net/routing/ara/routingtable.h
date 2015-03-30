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
 * @file        routingtable.h
 * @brief       ARA Routing Table
 *
 * @author      Michael Frey <frey@informatik.hu-berlin.de>
 */

#ifndef ARA_ROUTINGTABLE_H_
#define ARA_ROUTINGTABLE_H_

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
};

/** 
 * The ARA nexthop structure is used as an list element for the ARA routing table (i.e. a
 * destination might have multiple next hops to reach it)
 */                                                                                                                           
struct ara_nexthop_t 
{                                                                                                                                                     
    struct netaddr nextHop;     /**< the actual next hop */
    double phi;                 /**< pheromone value of the entry */
    double credit;              /**< how many times we may not receive an ack before the route is dropped */
    uint8_t ttl;                /**< TTL for this entry */
    struct ara_nexthop* prev;   /**< previous entry in the list */
    struct ara_nexthop* next;   /**< next entry in the list */
};     


#ifdef __cplusplus
}
#endif

#endif /* ARA_ROUTINGTABLE_H_*/
