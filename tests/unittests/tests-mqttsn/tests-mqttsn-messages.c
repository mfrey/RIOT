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

static void test_mqttsn_search_gateway(void) 
{
    mqttsn_search_gateway();
    TEST_ASSERT_EQUAL_INT(packet[0], 3);
    TEST_ASSERT_EQUAL_INT(packet[1], MQTTSN_TYPE_SEARCHGW);
    TEST_ASSERT_EQUAL_INT(packet[2], MQTTSN_DEFAULT_RADIUS);
}

static void test_mqttsn_search_gateway_with_specified_radius(void) 
{
    uint16_t broadcast_radius = 42;
    mqttsn_set_radius(broadcast_radius);
    mqttsn_search_gateway();
    TEST_ASSERT_EQUAL_INT(packet[0], 3);
    TEST_ASSERT_EQUAL_INT(packet[1], MQTTSN_TYPE_SEARCHGW);
    TEST_ASSERT_EQUAL_INT(packet[2], broadcast_radius);
}

static void test_mqttsn_disconnect(void) 
{
    mqttsn_disconnect();
    TEST_ASSERT_EQUAL_INT(packet[0], 2);
    TEST_ASSERT_EQUAL_INT(packet[1], MQTTSN_TYPE_DISCONNECT);
}

Test *tests_mqttsn_messages_tests(void) 
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture(test_mqttsn_search_gateway),
        new_TestFixture(test_mqttsn_disconnect),
        new_TestFixture(test_mqttsn_search_gateway_with_specified_radius),
        new_TestFixture(test_mqttsn_ping_request)
    };

    EMB_UNIT_TESTCALLER(mqttsn_messages_tests, setup, teardown, fixtures);

    return (Test *)&mqttsn_messages_tests;
}
