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
#include "reinforcement.h"


kernel_pid_t ara_pid = KERNEL_PID_UNDEF;

static char _ara_stack[ARA_STACK_SIZE];
/* main event loop for ARA */
static void *ara_event_loop(void *args);
/** the next sequence number */
static uint8_t next_sequence_number = 0;

kernel_pid_t ara_init(void)
{
    /* initialize the ara thread */
    if (ara_pid == KERNEL_PID_UNDEF) {
        ara_pid = thread_create(_ara_stack, sizeof(_ara_stack), ARA_PRIO,
                CREATE_STACKTEST, ara_event_loop, NULL, "ara");
    }
    /* initialize the probablistic forwarding */
    ara_forwarding_init();
    /* set the reinforcement to the linear reinforcement function */
    ara_reinforce = ara_reinforcement_linear_compute;

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

bool ara_is_ipv6_packet(gnrc_pktsnip_t* packet) 
{
    return (packet->type == GNRC_NETTYPE_IPV6);
}

void ara_send_packet(gnrc_pktsnip_t* packet)
{
    ipv6_hdr_t* header = (ipv6_hdr_t*)packet->data;
    ara_routing_table_trigger_evaporation();

    /** check the hop limit (former ttl) */
    if (header->hl > 0) {
        ipv6_addr_t* destination = &(header->dst);

        if (ara_is_route_discovery_running(destination)) {
            /* trap packet */
//                ara_packet_trap_trap_packet(packet);
        //} else if(ara_routing_table_is_deliverable(destination)) {
        } else if(ara_routing_table_is_deliverable(NULL)) {
            /* get next hop */
            //ara_next_hop_t* next_hop = ara_get_next_hop(destination);
            ara_next_hop_t* next_hop = ara_get_next_hop(NULL);
            /* build packet */

            /**TODO: set next hop */

            /* reinforce pheromone value */
            //ara_reinforce_pheromone(next_hop->address, destination);
            ara_reinforce_pheromone(next_hop->address, NULL);
            /* send packet */
            ara_send(packet);
        } else {
            /* packet is not deliverable and no route discovery is running */
            //if (ara_is_local_address(packet->source)) {
            if (ara_is_local_address(NULL)) {
                /* trap packet */
//                ara_packet_trap_trap_packet(packet);
                /* */
                ara_start_new_route_discovery(packet);
            } else {
                ara_handle_non_source_route_discovery(packet);
            }
        }
    /* handle packet with zero hop limit*/
    } else {
        ara_handle_packet_with_zero_hop_limit(packet);
    }
}

ara_next_hop_t* ara_get_next_hop(ipv6_addr_t* destination)
{
    ara_next_hop_t* result = NULL;
    /**
     * the forwarding functions require the routing table entry with all
     * possible next hops in order to (probalistic) chose from all options.
     * hence, we determine the corresponding entry for a given address.
     */
    //ara_routing_entry_t* entry = ara_routing_table_get_entry(destination);
    ara_routing_entry_t* entry = ara_routing_table_get_entry(NULL);

    if (entry != NULL) {
       switch (ARA_FORWARDING_MODE) {
           default:
               result = ara_stochastic_forwarding(entry);
       }
    } else {
#if ENABLE_DEBUG
        ipv6_addr_t_str buf;
        printf("there is no routing table entry for %s\n", netaddr_to_string(&buf, destination));
#endif
    }

    return result;
}


void ara_reinforce_pheromone(ipv6_addr_t* next_hop, ipv6_addr_t* destination)
{
    float phi = ara_routing_table_get_pheromone_value(destination, next_hop);
    ara_routing_table_update(destination, next_hop, ara_reinforce(phi));
}


bool ara_is_route_discovery_running(ipv6_addr_t* destination)
{
    return false;
}

void ara_handle_non_source_route_discovery(gnrc_pktsnip_t *packet)
{
    ipv6_hdr_t* header = (ipv6_hdr_t*)packet->data;
    ipv6_addr_t *destination = &(header->dst);
    ara_broadcast_route_failure(destination);
    gnrc_pktbuf_release(packet);
}

void ara_broadcast_route_failure(ipv6_addr_t* destination)
{
    uint8_t seq_nr = ara_get_next_sequence_number();
    /** determine the source address */
    ipv6_addr_t* source = ara_get_source_address(); 
    /** ip stack determines the correct address */
    gnrc_pktsnip_t* route_failure_packet = ara_packets_make_route_failure_packet(source, destination, seq_nr);
    /** send the packet */
    ara_send(route_failure_packet);
}


void ara_broadcast_fant(ipv6_addr_t* destination)
{
    uint8_t seq_nr = ara_get_next_sequence_number();
    /** determine the source address */
    ipv6_addr_t* source = ara_get_source_address(); 
    /** ip stack determines the correct address */
    gnrc_pktsnip_t* fant = ara_packets_make_fant(source, destination, seq_nr);
    /** send the packet */
    ara_send(fant);
}

void ara_send(gnrc_pktsnip_t* packet) 
{
    if (!gnrc_netapi_dispatch_send(GNRC_NETTYPE_IPV6, GNRC_NETREG_DEMUX_CTX_ALL, packet)) {
        /** unable to locate ip thread */
        gnrc_pktbuf_release(packet);
    }
}


ipv6_addr_t* ara_get_source_address(void) 
{
    /** TODO: gnrc_ipv6_netif_find_best_...*/
    return NULL;
}

void ara_handle_packet_with_zero_hop_limit(gnrc_pktsnip_t* packet)
{
    /** if the hop limit/ttl is exceeded, we delete the packet  */
    gnrc_pktbuf_release(packet);
}


void ara_handle_packet(void)
{

}


void ara_handle_ant_packet(void)
{

}


uint8_t ara_get_next_sequence_number(void) 
{
    return next_sequence_number++;
}


