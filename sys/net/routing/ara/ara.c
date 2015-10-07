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
 * @file        ara.c
 * @brief       ARA routing protocol
 *
 * @author      Michael Frey <frey@informatik.hu-berlin.de>
 */

#include "ara.h"
#include "msg.h"
#include "forwarding.h"
#include "routingtable.h"


kernel_pid_t ara_pid = KERNEL_PID_UNDEF;

static char _ara_stack[ARA_STACK_SIZE];
/* main event loop for ARA */
static void *ara_event_loop(void *args);

kernel_pid_t ara_init(void)
{
    /* initialize the ara thread */
    if (ara_pid == KERNEL_PID_UNDEF) {
        ara_pid = thread_create(_ara_stack, sizeof(_ara_stack), ARA_PRIO,
                CREATE_STACKTEST, ara_event_loop, NULL, "ara");
    }
    /* initialize the probablistic forwarding */
    ara_forwarding_init();

    return ara_pid;
}

static void *ara_event_loop(void *args)
{
    msg_t message; 
    msg_t message_queue[ARA_MSG_QUEUE_SIZE];

    if(msg_init_queue(message_queue, ARA_MSG_QUEUE_SIZE) == -1){

    }

    while (1) {
        msg_receive(&message);

        switch (message.type) {
            default:
                break;
        }
    }

    return NULL;
}

void ara_send_packet(ara_packet_t* packet)
{
    ara_routing_table_trigger_evaporation();

    if (packet->ttl > 0) {
        struct netaddr* destination = packet->destination;

        if (ara_is_route_discovery_running(destination)) {
            /* trap packet */

        } else if(ara_routing_table_is_deliverable(destination)) {
            /* get next hop */
            ara_next_hop_t* next_hop = ara_get_next_hop(destination);
            /* build packet */

            /* reinforce pheromone value */
//            float phi = ara_reinforce_pheromone(next_hop->address, destination, NULL);
#if ENABLE_DEBUG
            struct netaddr_str buf;
            printf("there is no routing table entry for %s\n", netaddr_to_string(&buf, destination));
#endif
   
            /* send packet */

        } else {
            /* packet is not deliverable and no route discovery is running */
            if (ara_is_local_address(packet->source)) {
                /* packet from is not deliverable */

                /* trap packet */

                /* */
                ara_start_new_route_discovery(packet);
            } else {
                ara_handle_non_source_route_discovery(packet);
            }

        }
    /* handle packet with zero ttl  */
    } else {
        ara_handle_packet_with_zero_ttl(packet);
    }
}

ara_next_hop_t* ara_get_next_hop(struct netaddr* destination)
{
    ara_next_hop_t* result = NULL;
    /**
     * the forwarding functions require the routing table entry with all
     * possible next hops in order to (probalistic) chose from all options.
     * hence, we determine the corresponding entry for a given address.
     */
    ara_routing_entry_t* entry = ara_routing_table_get_entry(destination);

    if (entry != NULL) {
       switch (ARA_FORWARDING_MODE) {
           default:
               result = ara_stochastic_forwarding(entry);
       }
    } else {
#if ENABLE_DEBUG
        struct netaddr_str buf;
        printf("there is no routing table entry for %s\n", netaddr_to_string(&buf, destination));
#endif
    }

    return result;
}


void ara_reinforce_pheromone(struct netaddr* next_hop, struct netaddr* destination, void* interface)
{
    /* 
    float current_value = ara_routing_table_get_pheromone_value(destination, next_hop);
    float new_pheromone_value = ara_reinforcement_linear_compute(current_pheromone_value);
    ara_routing_table_update(destination, next_hop, interface, new_pheromone_value);
    return new_pheromone_value;
     */
}


bool ara_is_route_discovery_running(struct netaddr* destination)
{
    return false;
}

void ara_handle_non_source_route_discovery(ara_packet_t *packet)
{
    // Dropping packet ....
    ara_broadcast_route_failure(packet->destination);
    //free packet
}

void ara_broadcast_route_failure(struct netaddr* address)
{

}

void ara_handle_packet_with_zero_ttl(ara_packet_t* packet)
{
    // Dropping packet ...
    // free(packet);
}

