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

/**
 * Initializes variables responsible for handling WILL topics and messages.
 *
 * @param[in] will_topic The WILL topic to set
 * @param[in] topic_length The length of the WILL topic
 * @param[in] will_msg The WILL message to set
 * @param[in] message_length The length of the WILL message
 */
void mqttsn_will_init(char *will_topic, size_t topic_length, char *will_msg, size_t message_length);

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

/**
 * Updates or sets a new WILL topic.
 *
 * @param[in] topic The topic to be set or updated.
 * @param[in] topic_length The length of the new or updated topic.
 *
 * @return 0 upon success, -1 otherwise
 */
int8_t mqttsn_will_set_topic(char *topic, size_t length);

/**
 * Updates or sets a new WILL message.
 *
 * @param[in] topic The message to be set or updated.
 * @param[in] topic_length The length of the new or updated message.
 *
 * @return 0 upon success, -1 otherwise
 */
int8_t mqttsn_will_set_message(char *message, size_t length);

uint8_t mqttsn_will_msg_size(void);

uint8_t mqttsn_will_topic_size(void);

void mqttsn_will_clear(void);

#ifdef __cplusplus
}
#endif

#endif /* MQTTSN_WILL_H_ */
