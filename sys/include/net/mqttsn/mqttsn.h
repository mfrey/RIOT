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

#include "kernel_types.h"
#include "net/ipv6/addr.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes MQTT-SN.
 *
 * TODO: detailed description
 *
 * @brief[in] src The source address of the client.
 * @brief[in] src_port The port MQTT-SN should run.
 * @brief[in] enable_forward_encapsulation Indicates if forward encapsulation should be enabled.
 * @brief[in] qos The desired QoS level.
 * @brief[in] device_identifier The wireless node identifier of this device.
 * @brief[in] size The size of the wireless node identifier of this device.
 *
 * @return On success the PID of the thread which controls the mqtt-sn client,
 * -EINVAL, if the priority is greater than or equal to SCHED_PRIO_LEVELS, or 
 * -EOVERFLOW, if there are too many threads already running
 */
kernel_pid_t mqttsn_init(ipv6_addr_t src, uint16_t src_port, bool enable_forward_encapsulation, int8_t qos, uint8_t *device_identifier, uint8_t size);

#ifdef __cplusplus
}
#endif

#endif /* MQTTSN_H_ */
