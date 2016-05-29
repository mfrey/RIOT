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

static void test_mqttsn_connect(void) 
{
    uint8_t duration = 42;
    // TODO: replace with client_identifier function 
    const char *client_identifier = "RIOT";
    /** length of the client identifier */
    size_t length = strlen(client_identifier);

    mqttsn_connect(client_identifier, length, false, duration, true);
    /** check the size of the packet */
    TEST_ASSERT_EQUAL_INT(packet[0], 0x06+length);
    /** check the type of the packet */
    TEST_ASSERT_EQUAL_INT(packet[1], MQTTSN_TYPE_CONNECT);
    /** check the flags of the packet */
    TEST_ASSERT_EQUAL_INT(packet[2], MQTTSN_FLAG_CLEAN_SESSION);
    /** check the protocol version */
    TEST_ASSERT_EQUAL_INT(packet[3], MQTTSN_PROTOCOL_VERSION);
    /** check the duration */
    TEST_ASSERT_EQUAL_INT(*((uint16_t*)&(packet[4])), HTONS(duration));
    /** check the client id */
    TEST_ASSERT_EQUAL_INT(0, strncmp((char*)(&(packet[6])), "RIOT", 4));

    /** reset the packet */
    memset(packet, 0, MQTTSN_MAX_PACKET_LENGTH);
    /** enable will and clean session  */
    mqttsn_connect(client_identifier, length, true, duration, true);
    /** check the type of the packet */
    TEST_ASSERT_EQUAL_INT(packet[1], MQTTSN_TYPE_CONNECT);
    /** check the flags of the packet */
    TEST_ASSERT_EQUAL_INT(packet[2], MQTTSN_FLAG_CLEAN_SESSION + MQTTSN_FLAG_WILL);

    /** reset the packet */
    memset(packet, 0, MQTTSN_MAX_PACKET_LENGTH);
    /** enable will and disable clean session  */
    mqttsn_connect(client_identifier, length, true, duration, false);
    /** check the type of the packet */
    TEST_ASSERT_EQUAL_INT(packet[1], MQTTSN_TYPE_CONNECT);
    /** check the flags of the packet */
    TEST_ASSERT_EQUAL_INT(packet[2], MQTTSN_FLAG_WILL);
}

/**
 * Mimics the MQTT-SN connect procedure
 *
 * client                       gateway
 *    | CONNECT                    |
 *    |--------------------------->|
 *    |               WILLTOPICREQ |
 *    |<---------------------------|
 *    | WILLTOPIC                  |
 *    |--------------------------->|
 *    |                 WILLMSGREQ |
 *    |<---------------------------|
 *    | WILLMSG                    |
 *    |--------------------------->|
 *    |                    CONNACK |
 *    |<---------------------------|
 *
 */
static void test_mqttsn_connect_procedure(void) 
{
    /** 
     * don't "send" a connect message, since it is handled by the gateway,
     * but mimic the reception of a WILLTOPICREQ
     */
    packet[1] = MQTTSN_TYPE_WILLTOPICREQ;
    /** simulate reception of a WILLTOPICREQ */
    mqttsn_handle_msg(packet, NULL);
    /** client answers by means of WILLTOPIC */
    TEST_ASSERT_EQUAL_INT(packet[1], MQTTSN_TYPE_WILLTOPIC);

    /** gateway sends a WILLMSGREQ */
    packet[1] = MQTTSN_TYPE_WILLMSGREQ;
    /** simulate reception of a WILLMSGREQ */
    mqttsn_handle_msg(packet, NULL);
    /** client answers by means of WILLTOPIC */
    TEST_ASSERT_EQUAL_INT(packet[1], MQTTSN_TYPE_WILLMSG);
}

static void test_mqttsn_ping_request(void) 
{
    mqttsn_ping_request();
    TEST_ASSERT_EQUAL_INT(packet[0], 2);
    TEST_ASSERT_EQUAL_INT(packet[1], MQTTSN_TYPE_PINGREQ);
}

static void test_mqttsn_ping_response(void) 
{
    mqttsn_ping_response();
    TEST_ASSERT_EQUAL_INT(packet[0], 2);
    TEST_ASSERT_EQUAL_INT(packet[1], MQTTSN_TYPE_PINGRESP);
}

static void test_mqttsn_ping_request_handling(void)
{
    mqttsn_ping_request();
    TEST_ASSERT_EQUAL_INT(packet[0], 2);
    TEST_ASSERT_EQUAL_INT(packet[1], MQTTSN_TYPE_PINGREQ);
    /** should trigger a ping response */
    mqttsn_handle_msg(packet, NULL);
    TEST_ASSERT_EQUAL_INT(packet[0], 2);
    TEST_ASSERT_EQUAL_INT(packet[1], MQTTSN_TYPE_PINGRESP);
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
        new_TestFixture(test_mqttsn_connect),
        new_TestFixture(test_mqttsn_connect_procedure),
        new_TestFixture(test_mqttsn_search_gateway),
        new_TestFixture(test_mqttsn_disconnect),
        new_TestFixture(test_mqttsn_search_gateway_with_specified_radius),
        new_TestFixture(test_mqttsn_ping_request),
        new_TestFixture(test_mqttsn_ping_request_handling),
        new_TestFixture(test_mqttsn_ping_response)
    };

    EMB_UNIT_TESTCALLER(mqttsn_messages_tests, setup, teardown, fixtures);

    return (Test *)&mqttsn_messages_tests;
}
