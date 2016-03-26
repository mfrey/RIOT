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
 * @file        device.c
 * @brief       Provides information about the device for mqtt-sn 
 *
 * @author      Michael Frey <mail@mfrey.net>
 */

#include "device.h"

#include "timex.h"
#include "xtimer.h"
#include "random.h"

#include <string.h>


static uint8_t wireless_node_id[MQTTSN_MAX_WIRELESS_NODE_ID_LENGTH];  
static uint8_t wireless_node_length;
static void mqttsn_device_random_init(void);

void mqttsn_device_init(uint8_t *node_id, uint8_t node_length) {
    /** check if the given node id and length is valid */
    if (mqttsn_device_validate(node_id, node_length) == 0) {
        /** set the device id */
        memcpy(wireless_node_id, node_id, node_length);
        /** update the length */
        wireless_node_length = node_length;
    } else { 
        /** create a new device id */
        mqttsn_device_create();
    }
}

uint8_t mqttsn_device_validate(uint8_t *node_id, uint8_t node_length) {
    if (node_length > MQTTSN_MAX_WIRELESS_NODE_ID_LENGTH) {
#if ENABLE_DEBUG 
        printf("the size of the wireless node identifier is too large. got: %d but maximum length is: %d !\n", node_length, MQTTSN_MAX_WIRELESS_NODE_ID_LENGTH);
#endif 
        return -1;
    }

    if ((!node_id) || (node_length == 0)) {
        // TODO
//        snprintf(wireless_node_id, sizeof(wireless_node_id)-1, "%X", mqttsn_get_wireless_node_id(void));
  //      wireless_node_length = strnlen((char*)packet->wireless_node_id, MQTTSN_MAX_WIRELESS_NODE_ID_LENGTH);
   // } else {
//        memcpy(packet->wireless_node_id, wireless_node_id, wireless_node_length);
    }

    return 0;
}

void mqttsn_device_create(void) {
    /** initialize the wireless node id with '0' */
    memset(wireless_node_id, 0, sizeof(uint8_t) * MQTTSN_MAX_WIRELESS_NODE_ID_LENGTH);
    /** initialize random number generator */
    mqttsn_device_random_init();
    /** obtain a random number which is going to be used as a device id */
    uint32_t id = random_uint32();
    /** set the wireless node length */
    wireless_node_length = sizeof(id);
    /** set the id */
    memcpy(wireless_node_id, &id, wireless_node_length);
}

uint8_t mqttsn_device_get_node_length(void)
{
    return wireless_node_length;
}

uint8_t* mqttsn_device_get_nodh(void)
{
    return wireless_node_id;
}

static void mqttsn_device_random_init(void) 
{
    /**
     * TODO: should spent some thoughts about the sizes of data types, rng's and
     * the size of the constants/packet size
     */
    timex_t now;
    xtimer_now_timex(&now);
    random_init(timex_uint64(now));
}
