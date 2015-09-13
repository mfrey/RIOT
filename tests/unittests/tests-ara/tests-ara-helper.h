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
 * @file        tests-ara-heler.h
 * @brief       Helper functions for the tests for the ``ara`` module
 *
 * @author      Michael Frey <frey@informatik.hu-berlin.de>
 */
#ifndef TESTS_ARA_HELPER_H_
#define TESTS_ARA_HELPER_H_

#include "embUnit.h"

#include <math.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

bool approximately_equal(float a, float b, float epsilon);

#ifdef __cplusplus
}
#endif

#endif /* TESTS_ARA_HELPER_H_ */
/** @} */
