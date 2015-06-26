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

#ifdef __cplusplus
extern "C" {
#endif

void ara_init(void);

void ara_send_packet(ara_packet_t* packet);

/*
void ara_receive_packet();
*/

bool ara_is_route_discovery_running(struct netaddr* destination);

bool ara_is_deliverable(struct netaddr* destination);

#ifdef  __cplusplus
}
#endif

#endif /* ARA_H_ */
