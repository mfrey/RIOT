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
 * @file        device.h
 * @brief       Provides information about the device for mqtt-sn 
 *
 * @author      Michael Frey <mail@mfrey.net>
 */

#ifndef MQTTSN_DEVICE_H_
#define MQTTSN_DEVICE_H_

#include "stdint.h"

#include "net/mqttsn/constants.h"

#ifdef __cplusplus
extern "C" {
#endif

void mqttsn_device_init(uint8_t *node_id, uint8_t node_length);

uint8_t mqttsn_device_validate(uint8_t *node_id, uint8_t node_length);

uint8_t mqttsn_device_get_node_length(void);

uint8_t* mqttsn_device_get_node(void);

void mqttsn_device_create(void);

#ifdef __cplusplus
}
#endif

#endif /* MQTTSN_DEVICE_H_ */
