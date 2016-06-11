/*
 * Copyright (C) 2016 Michael Frey <mail@mfrey.net>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include "tests-mqttsn.h"
#include "string.h"
#include "mqttsn/will.h"

static void teardown(void)
{
    mqttsn_will_clear();
}

static void test_mqttsn_will_init(void)
{
    char* topic = "some topic";
    size_t topic_length = strnlen(topic, 12);

    char* will = "some will";
    size_t will_length = strnlen(topic, 10);

    /** set the topic and will */
    mqttsn_will_init(topic, topic_length, will, will_length);
    /** should match the length of the topic/will */
    TEST_ASSERT_EQUAL_INT(will_length, mqttsn_will_msg_size());
    TEST_ASSERT_EQUAL_INT(topic_length, mqttsn_will_topic_size());
}

static void test_mqttsn_will_set_topic(void)
{
    char* topic_is_too_long = "a topic which is too long";
    size_t new_topic_length = strnlen(topic_is_too_long, 26);

    /** try to update the topic */
    int8_t result = mqttsn_will_set_topic(topic_is_too_long, new_topic_length);
    /** 
     * since the new topic is too large, we stick with the old topic. if no
     * topic has been set, the size is MQTTSN_DEFAULT_WILL_TOPIC_SIZE
     *
     */
    TEST_ASSERT_EQUAL_INT(result, -1);
    TEST_ASSERT_EQUAL_INT(MQTTSN_DEFAULT_WILL_TOPIC_SIZE, mqttsn_will_topic_size());
}

static void test_mqttsn_will_set_message(void)
{
    char* will_is_too_long = "a will which is too long";
    size_t new_will_length = strnlen(will_is_too_long, 25);

    /** try to update the topic */
    int8_t result = mqttsn_will_set_message(will_is_too_long, new_will_length);
    /** 
     * since the new will is too large, we stick with the old will. if no
     * will has been set, the size is MQTTSN_DEFAULT_WILL_MESSAGE_SIZE
     *
     */
    TEST_ASSERT_EQUAL_INT(result, -1);
    TEST_ASSERT_EQUAL_INT(MQTTSN_DEFAULT_WILL_MESSAGE_SIZE, mqttsn_will_msg_size());
}

Test *tests_mqttsn_will_tests(void) 
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture(test_mqttsn_will_init),
        new_TestFixture(test_mqttsn_will_set_topic),
        new_TestFixture(test_mqttsn_will_set_message)
    };

    EMB_UNIT_TESTCALLER(mqttsn_will_tests, NULL, teardown, fixtures);

    return (Test *)&mqttsn_will_tests;
}
