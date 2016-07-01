/*
 * Copyright (C) 2016 Michael Frey <mail@mfrey.net>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include "tests-mqttsn.h"

#include "mqttsn/topic.h"

static void test_mqttsn_topic_add(void)
{

}

static void test_mqttsn_topic_valid_identifier(void)
{
//    uint16_t valid_topic_identifier = 0x0001;
//    TEST_ASSERT_EQUAL_INT(0, mqttsn_topic_valid_identifier(valid_topic_identifier));
}

Test *tests_mqttsn_topic_tests(void) 
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture(test_mqttsn_topic_add),
        new_TestFixture(test_mqttsn_topic_valid_identifier)
    };

    EMB_UNIT_TESTCALLER(mqttsn_topic_tests, NULL, NULL, fixtures);

    return (Test *)&mqttsn_topic_tests;
}
