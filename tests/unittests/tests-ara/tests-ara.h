/*
 * Copyright (C) 2015 Michael Frey <frey@informatik.hu-berlin.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @addtogroup  unittests
 * @{
 *
 * @file        tests-ara.h
 * @brief       Unittests for the ``ara`` module
 *
 * @author      Michael Frey <frey@informatik.hu-berlin.de>
 */
#ifndef TESTS_ARA_H_
#define TESTS_ARA_H_

#include "embUnit.h"

#include "net/routing/ara/helper.h"
#include "net/routing/ara/constants.h"
#include "net/routing/ara/routingtable.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   The entry point of this test suite.
 */
void tests_ara(void);

/**
 * @brief   Generates tests for the helper functions of ARA
 *
 * @return  embUnit tests if successful, NULL if not.
 */
Test *tests_ara_helper_tests(void);

/**
 * @brief   Generates tests for the evaporation functions of ARA
 *
 * @return  embUnit tests if successful, NULL if not.
 */
Test *tests_ara_evaporation_tests(void);

/**
 * @brief   Generates tests for the reinforcement functions of ARA
 *
 * @return  embUnit tests if successful, NULL if not.
 */
Test *tests_ara_reinforcement_tests(void);

/**
 * @brief   Generates tests for the routing table functions of ARA
 *
 * @return  embUnit tests if successful, NULL if not.
 */
Test *tests_ara_routingtable_tests(void);

/**
 * @brief   Generates tests for the packet trap  functions of ARA
 *
 * @return  embUnit tests if successful, NULL if not.
 */
Test *tests_ara_packet_trap_tests(void);

/**
 * @brief   Generates tests for ARA
 *
 * @return  embUnit tests if successful, NULL if not.
 */
Test *tests_ara_tests(void);

#ifdef __cplusplus
}
#endif

#endif /* TESTS_ARA_H_ */
/** @} */
