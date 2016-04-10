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
 * @file        data.h
 * @brief       Functions to handle data
 *
 * @author      Michael Frey <mail@mfrey.net>
 */


#ifndef MQTTSN_DATA_H_
#define MQTTSN_DATA_H_

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

void mqttsn_data_init(void);

void mqttsn_data_receive(void *data, size_t size);

/**
 * Sets the function pointer for processing data. The specification 
 * does not make any assumptions about how data received for a MQTT-SN
 * client is stored. The function pointer enables 3rd party applications
 * to provide their own data management for published MQTT-SN messages.
 *
 * @param[in] function A function pointer for receiving subscribed data.
 */
void mqttsn_set_data_process(void (*function)(void*, size_t));

#ifdef __cplusplus
}
#endif

#endif /* MQTTSN_DATA_H_ */
