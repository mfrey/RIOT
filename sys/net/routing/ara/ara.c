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
#include "forwarding.h"

void ara_init(void)
{
    /* initialize the probablistic forwarding */
    ara_forwarding_init();
}

void ara_send_packet(ara_packet_t* packet)
{
    if (packet->ttl > 0) {
        struct netaddr* destination = packet->destination;

        if (ara_is_route_discovery_running(destination)) {

        } else if(ara_is_deliverable(destination)) {

        }
    /* handle packet with zero ttl  */
    } else {


    }
}

bool ara_is_route_discovery_running(struct netaddr* destination)
{
    return false;
}

bool ara_is_deliverable(struct netaddr* destination)
{
    return false;
}
