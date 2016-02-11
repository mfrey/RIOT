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

#include "stdint.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

void ara_packet_trap_init(void);

/**
 * @brief Stores the given packet in the packet trap until it is removed via
 * untrapDeliverablePackets(..). The order of trapped packets is preserved in
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
uint16_t ara_packet_trap_count(void);


#ifdef __cplusplus
}
#endif

#endif /* ARA_PACKET_TRAP_H_ */
