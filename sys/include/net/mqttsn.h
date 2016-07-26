/*
 * Copyright (C) 2016 Michael Frey
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    mqtt-sn MQTT-SN 
 * @ingroup     mqtt-sn
 * @brief       Provides MQTT-SN types and functions 
 * @see         <a href="http://mqtt.org/new/wp-content/uploads/2009/06/MQTT-SN_spec_v1.2.pdf">
 *                  MQTT-SN
 *              </a>
 * @{
 *
 * @file
 * @brief   MQTT-SN types and function definitions
 *
 * @author  Michael Frey <mail@mfrey.net>
 */
#ifndef MQTT_SN_H_
#define MQTT_SN_H_

#include "net/ipv6/addr.h"

#ifdef __cplusplus
extern "C" {
#endif

void mqttsn_lala(void);

/**
 * @brief Initializes MQTT-SN.
 *
 * TODO: detailed description
 *
 * @brief[in] src The 
 * @brief[in] src_port
 * @brief[in] dest
 * @brief[in] dest_port 
 * @brief[in] enable_forward_encapsulation Indicates if forward encapsulation should be enabled.
 * @brief[in] qos The desired QoS level.
 */
void mqttsn_init(ipv6_addr_t src, uint16_t src_port, ipv6_addr_t dest, uint16_t dest_port, bool enable_forward_encapsulation, int8_t qos);

#ifdef __cplusplus
}
#endif

#endif /* MQTTSN_H_ */
