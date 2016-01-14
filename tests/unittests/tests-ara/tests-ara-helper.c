/*
 * Copyright (C) 2015 Michael Frey <frey@informatik.hu-berlin.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */
#include "tests-ara.h"

#include "net/routing/ara/helper.h"

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


Test *tests_ara_helper_tests(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture(test_ara_cum_sum)
    };

    EMB_UNIT_TESTCALLER(ara_helper_tests, NULL, NULL, fixtures);

    return (Test *)&ara_helper_tests;
}
