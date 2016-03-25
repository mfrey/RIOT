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
 * @file        debug.h
 * @brief       Helper functions to debug mqtt-sn 
 *
 * @author      Michael Frey <mail@mfrey.net>
 */

#ifndef MQTTSN_DEBUG_H_
#define MQTTSN_DEBUG_H_

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Returns a string representation of a given MQTT-SN return code.
 *
 * @param[in] return_code The return code to transform into a string.
 *
 * @return Upon success the string representation of the given return code, on
 * failure a message string containing a error message.
 */
const char* mqttsn_debug_return_code_to_string(uint8_t return_code);

/**
 * Returns a string representation of a given MQTT-SN message type.
 *
 * @param[in] msg_type The message type to transform into a string.
 *
 * @return Upon success the string representation of the given message type, on
 * failure a message string containing a error message.
 */
const char* mqttsn_debug_msg_type_to_string(uint8_t msg_type);

#ifdef __cplusplus
}
#endif

#endif 
