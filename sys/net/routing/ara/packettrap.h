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
 * @file        packettrap.h
 * @brief       packet trap for ARA 
 *
 * @author      Michael Frey <frey@informatik.hu-berlin.de>
 */

#ifndef ARA_PACKET_TRAP_H_
#define ARA_PACKET_TRAP_H_

#include "uthash.h"
#include "utlist.h"
#include "stdint.h"
#include "stdbool.h"

#include "net/gnrc.h"
#include "common/netaddr.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * The ARA trapped packet structure is used as an list element for the ARA packet trap.
 */                                                                                                                           
struct ara_packet_trap_trapped_packet_s 
{                                       
    gnrc_pktsnip_t *packet;                        /**< the packet which is trapped */
    struct ara_packet_trap_trapped_packet_s* prev; /**< previous entry in the list */
    struct ara_packet_trap_trapped_packet_s* next; /**< next entry in the list */
};

typedef struct ara_packet_trap_trapped_packet_s ara_packet_trap_trapped_packet_t;

/** 
 * ARA packet trap entry 
 */
struct ara_packet_trap_entry_s
{
    ipv6_addr_t* destination;                  /**< destination address */
    ara_packet_trap_trapped_packet_t* packets; /**< a list of packets to the destination */
    UT_hash_handle hh;
};

typedef struct ara_packet_trap_entry_s ara_packet_trap_entry_t;

void ara_packet_trap_init(void);

/**
 * @brief Stores the given packet in the packet trap until it is removed via
 * ara_packt_trap_untrap_deliverable_packets(..). The order of trapped packets is preserved in
 * a FIFO style.
 */
void ara_packet_trap_trap_packet(void);

/**
 * @brief Returns true if this packet trap contains a given packet, false otherwise.
 */
bool ara_packet_trap_contains(void);

/**
 * @brief  Returns the number of trapped packets for a given destination or the
 * total number of all trapped packets if destination is null.
 */
uint8_t ara_packet_trap_count(ipv6_addr_t* destination);


#ifdef __cplusplus
}
#endif

#endif /* ARA_PACKET_TRAP_H_ */
