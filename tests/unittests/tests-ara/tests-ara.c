/*
 * Copyright (C) 2015 Michael Frey <frey@informatik.hu-berlin.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include "tests-ara.h"

void tests_ara(void)
{
    TESTS_RUN(tests_ara_helper_tests());
    TESTS_RUN(tests_ara_routingtable_tests());
    TESTS_RUN(tests_ara_evaporation_tests());
    TESTS_RUN(tests_ara_reinforcement_tests());
    TESTS_RUN(tests_ara_packet_trap_tests());
}
