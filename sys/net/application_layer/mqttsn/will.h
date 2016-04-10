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
 * @file        will.h
 * @brief       Handles WILL topics and messages of mqtt-sn 
 *
 * @author      Michael Frey <mail@mfrey.net>
 */

#ifndef MQTTSN_WILL_H_
#define MQTTSN_WILL_H_

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

void mqttsn_will_init(char *will_topic, size_t topic_length, char *will_msg, size_t message_length);

/**
 * Sets the WILL message of the client.
 *
 * @return On success 0, or -1 if the message could not be set
 */
uint8_t mqttsn_will_set_message(char *message, size_t length);

/**
 * Sets the WILL topic of the client.
 *
 * @return On success 0, or -1 if the of could not be set
 */
uint8_t mqttsn_will_set_topic(char *topic, size_t length);

/**
 * Returns the WILL message set by the client.
 *
 * @return On success the will message of the client, otherwise NULL.
 */
char* mqttsn_will_get_message(void);

/**
 * Returns the WILL topic set by the client.
 *
 * @return On success the will topic of the client, otherwise NULL.
 */
char* mqttsn_will_get_topic(void);


#ifdef __cplusplus
}
#endif

#endif /* MQTTSN_WILL_H_ */
