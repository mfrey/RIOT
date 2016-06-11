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
 * @file        will.c
 * @brief       Handles WILL topics and messages of mqtt-sn 
 *
 * @author      Michael Frey <mail@mfrey.net>
 */

#include <stdint.h>
#include <string.h>

#include "will.h" 
#include "constants.h"

/** The WILL message set by the client. */
static char will_msg[MQTTSN_DEFAULT_WILL_MESSAGE_SIZE];
/** The WILL topic set by the client. */
static char will_topic[MQTTSN_DEFAULT_WILL_TOPIC_SIZE];
/** The actual size of the WILL topic */
static size_t will_topic_size = MQTTSN_DEFAULT_WILL_TOPIC_SIZE;
/** The actual size of the WILL message */
static size_t will_msg_size = MQTTSN_DEFAULT_WILL_MESSAGE_SIZE;

/**
 * The function copies the content of a source array with a given size 
 * to a destination array. If the size of the source array is larger
 * than the destination array, an error code is returned. 
 *
 * @param[in] source The source array
 * @param[in] source_length The size of the source array
 * @param[in] destination The destination array
 * @param[in] destination_length The size of the destination array
 *
 * @return 0 if the operation was successful, -1 if the source array was larger
 * than the destination array.
 */
static int8_t mqttsn_will_set_char_array(char *source, size_t source_length, char *destination, size_t destination_length);


void mqttsn_will_init(char *topic, size_t topic_length, char *message, size_t message_length) 
{
    if (topic) {
        mqttsn_will_set_topic(topic, topic_length);
    }

    if (message) {
        mqttsn_will_set_message(message, message_length);
    }
}

void mqttsn_will_clear(void)
{
    memset(will_topic, 0, sizeof(char) * MQTTSN_DEFAULT_WILL_TOPIC_SIZE);
    will_topic_size = MQTTSN_DEFAULT_WILL_TOPIC_SIZE;
    memset(will_msg, 0, sizeof(char) * MQTTSN_DEFAULT_WILL_MESSAGE_SIZE);
    will_msg_size = MQTTSN_DEFAULT_WILL_MESSAGE_SIZE;
}

static int8_t mqttsn_will_set_char_array(char *source, size_t source_length, char *destination, size_t destination_length) 
{
    if (source_length <= destination_length) {
        strncpy(destination, source, source_length-1);

        if (source_length > 0) {
            destination[source_length-1] = '\0';
        }

        return 0;
    }

    return -1;
}

int8_t mqttsn_will_set_topic(char *topic, size_t topic_length)
{
    int8_t result = mqttsn_will_set_char_array(topic, topic_length, will_topic, MQTTSN_DEFAULT_WILL_TOPIC_SIZE);

    if (result == -1) {
#if ENABLE_DEBUG 
        printf("could not set the topic!\n");
#endif 
    } else {
        will_topic_size = topic_length;
    }

    return result;
}

int8_t mqttsn_will_set_message(char *msg, size_t msg_length)
{
    int8_t result = mqttsn_will_set_char_array(msg, msg_length, will_topic, MQTTSN_DEFAULT_WILL_TOPIC_SIZE);

    if (result == -1) {
#if ENABLE_DEBUG 
        printf("could not set message!\n");
#endif 
    } else {
        will_msg_size = msg_length;
    }

    return result;
}

uint8_t mqttsn_will_msg_size(void) 
{
    return will_msg_size;
}

uint8_t mqttsn_will_topic_size(void) 
{
    return will_topic_size;
}

char* mqttsn_will_get_message(void) 
{
    return will_msg;
}

char* mqttsn_will_get_topic(void)
{
    return will_topic;

}
