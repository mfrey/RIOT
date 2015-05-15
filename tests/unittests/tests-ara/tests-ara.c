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
    /*
    ara_routing_table_init();
    TEST_ASSERT_EQUAL_INT(ara_routing_table[0].nextHopListSize, 0);
    TEST_ASSERT_EQUAL_INT(ara_routing_table[0].lastAccessTime.seconds, 0);
    TEST_ASSERT_EQUAL_INT(ara_routing_table[0].lastAccessTime.microseconds, 0);
    */
}

static void test_ara_routing_table_add_entry(void)
{
    /*
    struct ara_routing_entry_t *entry = (struct ara_routing_entry_t*) malloc(sizeof(struct ara_routing_entry_t));

    if (entry) { 
        memset(entry, 0, sizeof(struct ara_routing_entry_t));
        ara_routing_table_add_entry(entry);

        TEST_ASSERT(&ara_routing_table[0] != entry);

        TEST_ASSERT_EQUAL_INT(ara_routing_table[0].nextHopListSize, entry->nextHopListSize);
        TEST_ASSERT_EQUAL_INT(ara_routing_table[0].lastAccessTime.seconds, entry->lastAccessTime.seconds);
        TEST_ASSERT_EQUAL_INT(ara_routing_table[0].lastAccessTime.microseconds, entry->lastAccessTime.microseconds);

        free(entry);
    }
*/
}

static void test_ara_cum_sum(void)
{
    float input[] = {2.,3.,4.};
    float output[] = {0.,0.,0.};

    /* call the function with size zero */
    ara_cum_sum(input, output, 0);
    /* nothing should change */
    TEST_ASSERT_EQUAL_INT(input[0], 2);
    TEST_ASSERT_EQUAL_INT(input[1], 3);
    TEST_ASSERT_EQUAL_INT(input[2], 4);

    TEST_ASSERT_EQUAL_INT(output[0], 0);
    TEST_ASSERT_EQUAL_INT(output[1], 0);
    TEST_ASSERT_EQUAL_INT(output[2], 0);

    ara_cum_sum(input, output, 3);
    /* TODO: no double/float check */
    TEST_ASSERT_EQUAL_INT(output[0], 2);
    TEST_ASSERT_EQUAL_INT(output[1], 5);
    TEST_ASSERT_EQUAL_INT(output[2], 9);
    
    ara_cum_sum(input, input, 3);
    TEST_ASSERT_EQUAL_INT(input[0], 2);
    TEST_ASSERT_EQUAL_INT(input[1], 5);
    TEST_ASSERT_EQUAL_INT(input[2], 9);
}


Test *tests_ara_tests(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture(test_ara_cum_sum),
        new_TestFixture(test_ara_routing_table_add_entry),
        new_TestFixture(test_ara_routing_table_initialized)
    };

    EMB_UNIT_TESTCALLER(ara_tests, NULL, NULL, fixtures);

    return (Test *)&ara_tests;
}

void tests_ara(void)
{
    TESTS_RUN(tests_ara_tests());
}
