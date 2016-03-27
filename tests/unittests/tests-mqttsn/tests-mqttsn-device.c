/*
 * Copyright (C) 2016 Michael Frey <mail@mfrey.net>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include "tests-mqttsn.h"

#include "net/application_layer/mqttsn/device.h"

static void test_mqttsn_device_get_node_length_test(void)
{

}

Test *tests_mqttsn_device_tests(void) 
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture(test_mqttsn_device_get_node_length_test)
    };

    EMB_UNIT_TESTCALLER(mqttsn_device_tests, NULL, NULL, fixtures);

    return (Test *)&mqttsn_device_tests;
}
