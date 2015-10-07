/*
 * Copyright (C) 2015 Michael Frey <frey@informatik.hu-berlin.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     ara 
 * @{
 *
 * @file        helper.h
 * @brief       Helper functions for ARA 
 *
 * @author      Michael Frey <frey@informatik.hu-berlin.de>
 */

#ifndef ARA_HELPER_H__
#define ARA_HELPER_H_

#include "stdint.h"
#include "stdbool.h"
#include "timex.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief     Computes the cumulated sum of the given double array and stores
 *            the results in the output array.
 * @param[in] input An array of doubles to sum up
 * @param[inout] output The result array of the cumulated sum
 * @param[in] size The size of the array
 */
void ara_cum_sum(float* input, float* output, uint8_t size);

bool ara_time_within_interval(timex_t timestamp, uint64_t interval_in_microseconds);

#ifdef __cplusplus
}
#endif

#endif /* ARA_HELPER_H_ */
