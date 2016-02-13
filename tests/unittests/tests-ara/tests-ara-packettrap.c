/*
 * Copyright (C) 2015 Michael Frey <frey@informatik.hu-berlin.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */
#include "tests-ara.h"
#include "tests-ara-util.h"

#include "net/routing/ara/packettrap.h"


static void test_ara_packet_trap_count(void) 
{

}

Test *tests_ara_packet_trap_tests(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture(test_ara_packet_trap_count)
    };

    EMB_UNIT_TESTCALLER(ara_packet_trap_tests, NULL, NULL, fixtures);

    return (Test *)&ara_packet_trap_tests;
}
