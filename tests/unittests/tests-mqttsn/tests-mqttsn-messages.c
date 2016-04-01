/*
 * Copyright (C) 2016 Michael Frey <mail@mfrey.net>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include "tests-mqttsn.h"

#include <string.h>

// Local global buffer for tests and set it in the dummy send method? 
char packet[MQTTSN_MAX_PACKET_LENGTH];


void tests_mqttsn_dummy_send(void *data) {
    size_t length = ((uint8_t*)data)[0];

    /** the length field is not 3 octets long */
    if (length != 0x01) {
        memcpy(packet, data, length);
    }
}

static void setup(void)
{
    /* set the the mqttsn_send function */
    mqttsn_set_mqttsn_send(tests_mqttsn_dummy_send);
}

static void teardown(void)
{
    memset(packet, 0, MQTTSN_MAX_PACKET_LENGTH);
}

static void test_mqttsn_ping_request(void) 
{
    mqttsn_ping_request();
    TEST_ASSERT_EQUAL_INT(packet[0], 2);
    TEST_ASSERT_EQUAL_INT(packet[1], MQTTSN_TYPE_PINGREQ);
}

static void test_mqttsn_messages_dummy(void)
{

}

Test *tests_mqttsn_messages_tests(void) 
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture(test_mqttsn_messages_dummy),
        new_TestFixture(test_mqttsn_ping_request)
    };

    EMB_UNIT_TESTCALLER(mqttsn_messages_tests, setup, teardown, fixtures);

    return (Test *)&mqttsn_messages_tests;
}
