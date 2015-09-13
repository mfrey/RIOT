/*
 * Copyright (C) 2015 Michael Frey <frey@informatik.hu-berlin.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */
#include "tests-ara.h"
#include "tests-ara-helper.h"

#include <stdio.h>

#include "net/routing/ara/reinforcement.h"


static void test_ara_reinforcement_path_reinforcement_via_routing_table_update(void) 
{
    float currentPhi = 5.1;
    float deltaPhi = 1.2;
    
    ara_reinforcement_init(deltaPhi);
    TEST_ASSERT(approximately_equal(currentPhi + deltaPhi, ara_reinforcement_linear_compute(currentPhi), 0.00001) == true);
}

Test *tests_ara_reinforcement_tests(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture(test_ara_reinforcement_path_reinforcement_via_routing_table_update)
    };

    EMB_UNIT_TESTCALLER(ara_reinforcement_tests, NULL, NULL, fixtures);

    return (Test *)&ara_reinforcement_tests;
}
