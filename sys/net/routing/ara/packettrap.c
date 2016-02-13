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
 * @file        packettrap.c
 * @brief       packet trap for ARA 
 *
 * @author      Michael Frey <frey@informatik.hu-berlin.de>
 */

#include "packettrap.h"

static ara_packet_trap_entry_t* ara_packet_trap = NULL;

void ara_packet_trap_init(void) 
{

}

void ara_packet_trap_trap_packet(void)
{

}

bool ara_packet_trap_contains(void) 
{
    return false;
}

uint8_t ara_packet_trap_count(ipv6_addr_t* destination) 
{
    ara_packet_trap_entry_t* entry= NULL;
    /** by default there is are no packets in the packet trap */
    uint8_t result = -1;
    /** find the entry in the packet trap */
    HASH_FIND(hh, ara_packet_trap, destination, sizeof(ipv6_addr_t), entry);

    if (entry!= NULL) {
        if (entry->packets != NULL) {
            ara_packet_trap_trapped_packet_t* packet;
            /** count the packets in the packet trap */
            DL_COUNT(entry->packets, packet, result);
        }
    }

    return result;
}
