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
 * @file        constants.h
 * @brief       Constants for MQTT-SN
 *
 * @author      Michael Frey <mail@mfrey.net>
 */

#ifndef MQTTSN_CONSTANTS_H_
#define MQTTSN_CONSTANTS_H_

#include "thread.h"
#include "kernel_types.h"

/**
 * The maxmimum packet length of a MQTT-SN message in bytes.
 */
#ifndef MQTTSN_MAX_PACKET_LENGTH
#define MQTTSN_MAX_PACKET_LENGTH (255)
#endif

/**
 * The maximum topic length. The constant '6' denotes the remaining 6 bytes
 * needed for the message.
 */
#ifndef MQTTSN_MAX_TOPIC_LENGTH 
#define MQTTSN_MAX_TOPIC_LENGTH (MQTTSN_MAX_PACKET_LENGTH - 6)
#endif

/**
 * The maximum length of the wireless node id (which is used in the 
 * forwarder encapsulation).
 */
#ifndef MQTTSN_MAX_WIRELESS_NODE_ID_LENGTH 
#define MQTTSN_MAX_WIRELESS_NODE_ID_LENGTH (252)
#endif

/**
 * The default value for ADVERTISE timers (in minutes). This refers
 * to the T_{ADV} value in the implementation notes of MQTT-SN.
 */
#ifndef MQTTSN_TIMER_ADVERTISE
#define MQTTSN_TIMER_ADVERTISE (15)
#endif

/**
 * The default value for ADVERTISE counter. This refers
 * to the N_{ADV} value in the implementation notes of MQTT-SN.
 */
#ifndef MQTTSN_COUNT_ADVERTISE
#define MQTTSN_COUNT_ADVERTISE (3)
#endif

/**
 * The default value for SEARCHGW timers (in seconds). This refers
 * to the T_{SEARCHGW} value in the implementation notes of MQTT-SN.
 */
#ifndef MQTTSN_TIMER_SEARCHGW 
#define MQTTSN_TIMER_SEARCHGW (5)
#endif

/**
 * The default value for GWINFO timers (in seconds). This refers
 * to the T_{GWINFO} value in the implementation notes of MQTT-SN.
 */
#ifndef MQTTSN_TIMER_GWINFO
#define MQTTSN_TIMER_GWINFO (5)
#endif

/**
 * The default value for wait timer (in minutes). This refers
 * to the T_{WAIT} value in the implementation notes of MQTT-SN.
 */
#ifndef MQTTSN_TIMER_TWAIT 
#define MQTTSN_TIMER_TWAIT (5)
#endif

/**
 * The default value for retry timer (in seconds). This refers
 * to the T_{RETRY} value in the implementation notes of MQTT-SN.
 */
#ifndef MQTTSN_TIMER_RETRY 
#define MQTTSN_TIMER_RETRY (10)
#endif

/**
 * The default value for number of retires counter. This refers
 * to the N_{ADV} value in the implementation notes of MQTT-SN.
 */
#ifndef MQTTSN_COUNT_RETRY 
#define MQTTSN_COUNT_RETRY (4)
#endif

/**
 * @brief   Default stack size to use for the MQTT-SN thread
 */
#ifndef MQTTSN_STACK_SIZE
#define MQTTSN_STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#endif

/**
 * @brief   Default priority for the MQTT-SN thread
 */
#ifndef MQTTSN_PRIO
#define MQTTSN_PRIO      (THREAD_PRIORITY_MAIN - 3)
#endif

/**
 * @brief   Default message queue size to use for the MQTT-SN thred.
 */
#ifndef MQTTSN_MSG_QUEUE_SIZE
#define MQTTSN_MSG_QUEUE_SIZE  (8U)
#endif

/**
 * @brief   Default broadcast radius for SEARCHGW, GWINFO MQTT-SN messages
 */
#ifndef MQTTSN_DEFAULT_RADIUS
#define MQTTSN_DEFAULT_RADIUS (1)
#endif

/**
 * @brief   Default number of gateways a MQTT-SN client can store  
 */
#ifndef MQTTSN_DEFAULT_GATEWAY_SIZE
#define MQTTSN_DEFAULT_GATEWAY_SIZE (5)
#endif

#endif 
