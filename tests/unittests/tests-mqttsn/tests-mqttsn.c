/*
 * Copyright (C) 2016 Michael Frey <mail@mfrey.net>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include "tests-mqttsn.h"

void tests_mqttsn(void)
{
    TESTS_RUN(tests_mqttsn_device_tests());
    TESTS_RUN(tests_mqttsn_messages_tests());
}
