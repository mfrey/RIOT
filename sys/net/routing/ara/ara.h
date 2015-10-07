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
 * @file        ara.h
 * @brief       ARA routing protocol
 *
 * @author      Michael Frey <frey@informatik.hu-berlin.de>
 */

#ifndef ARA_H_
#define ARA_H_

#include "stdbool.h"
#include "packet.h"
#include "routingtable.h"

#include "kernel_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Default stack size to use for the ARA thread
 */
#ifndef ARA_STACK_SIZE
#define ARA_STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#endif

/**
 * @brief   Default priority for the ARA thread
 */
#ifndef ARA_PRIO
#define ARA_PRIO      (THREAD_PRIORITY_MAIN - 3)
#endif

/**
 * @brief   Default evaporation interval 
 */
#ifndef ARA_EVAPORATION_INTERVAL
#define ARA_EvAPORATION_INTERVAL     (0)
#endif

/**
 * TODO: think about a better way to switch between forwarding modes
 */
#ifndef ARA_FORWARDING_MODE
#define ARA_FORWARDING_MODE (0)
#endif

/**
 * @brief   Default message queue size to use for the ARA thred.
 */
#ifndef ARA_MSG_QUEUE_SIZE
#define ARA_MSG_QUEUE_SIZE  (8U)
#endif

/**
 * @brief   Default parameter for linear reinforcement of pheromone values
 */
#ifndef ARA_LINEAR_REINFORCEMENT_DELTA
#define ARA_LINEAR_REINFORCEMNET_DELTA (0)
#endif

/**
 * TODO: think about a better way to switch between evaporation policies
 */
#ifndef ARA_EVAPORATION_POLICY
#define ARA_EVAPORATION_POLICY (0)
#endif

kernel_pid_t ara_init(void);

void ara_send_packet(ara_packet_t* packet);

/*
void ara_receive_packet();
*/

bool ara_is_route_discovery_running(struct netaddr* destination);

void ara_handle_non_source_route_discovery(ara_packet_t* packet);

void ara_handle_packet_with_zero_ttl(ara_packet_t* packet);

void ara_broadcast_route_failure(struct netaddr* address);

bool ara_is_local_address(struct netaddr* address);

void ara_start_new_route_discovery(ara_packet_t* packet);

bool ara_is_route_discovery_running(struct netaddr* address);

ara_next_hop_t* ara_get_next_hop(struct netaddr* destination);

void ara_reinforce_pheromone(struct netaddr* next_hop, struct netaddr* destination, void* interface);

float (*ara_reinforce)(float value);

#ifdef  __cplusplus
}
#endif

#endif /* ARA_H_ */
