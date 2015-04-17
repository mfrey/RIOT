/*
 * Copyright (C) 2015 Michael Frey <frey@informatik.hu-berlin.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include "tests-ara.h"

static void test_ara_routing_table_initialized(void)
{
    routingtable_init();
    TEST_ASSERT_EQUAL_INT(routing_table[0].nextHopListSize, 0);
}

/** FIXME: this is just a dummy */
static void test_ara_dummy(void)
{
    int i = 42;
//    TEST_ASSERT_EQUAL_INT(128, 128);
}

Test *tests_ara_tests(void)
{

    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture(test_ara_dummy),
        new_TestFixture(test_ara_routing_table_initialized)
    };

    EMB_UNIT_TESTCALLER(ara_tests, NULL, NULL, fixtures);

    return (Test *)&ara_tests;
}

void tests_ara(void)
{
    TESTS_RUN(tests_ara_tests());
}
