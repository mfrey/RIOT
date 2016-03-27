/*
 * Copyright (C) 2016 Michael Frey <mail@mfrey.net>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     mqtt-sn
 * @{
 *
 * @file        communication.h
 * @brief       Functions to send and receive MQTT-SN messages
 *
 * @author      Michael Frey <mail@mfrey.net>
 */

#ifndef MQTTSN_COMMUNICATION_H_
#define MQTTSN_COMMUNICATION_H_

#include <stdint.h>
#include <stdbool.h>

#include "net/ipv6/addr.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializes the MQTT-SN communication module
 *
 * @param[in] src The source host
 * @param[in] src_port The port at the source host
 * @param[in] dest The destination host
 * @param[in] dest_port The port at the destination host
 * @param[in] enable_forward_encapsulation A status flag which indicates if the
 * client provides forward encapsulation
 */
void mqttsn_communication_init(ipv6_addr_t src, uint16_t src_port, ipv6_addr_t dest, uint16_t dest_port, bool enable_forward_encapsulation);

/**
 * Provides means to send MQTT-SN messages over UDP.
 *
 * @param[in] packet The packet to sent over UDP
 */
void mqttsn_communication_send_udp(void *packet);

/**
 * Receives MQTT-SN messages over UDP. The function initializes a 
 * thread which listens for UDP packets provided by GNRC.
 */
void mqttsn_communication_receive_udp(void);

bool mqttsn_communication_is_forwarder_encapsulation_enabled(void);


#ifdef __cplusplus
}
#endif

#endif 
