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
 * @file        evaporation.c
 * @brief       Evaporation functions of ARA 
 *
 * @author      Michael Frey <frey@informatik.hu-berlin.de>
 */
#include "evaporation.h"

#include <math.h>

static uint64_t time_interval = 0;
static float threshold = .0;
static float evaporation_factor = .0;
static uint8_t evaporation_type = 0;

void ara_evaporation_init(float factor, float pheromone_threshold, uint64_t time_interval_milliseconds, uint8_t type)
{
    evaporation_factor = factor;
    threshold = pheromone_threshold;
    time_interval = time_interval_milliseconds;
    evaporation_type = type;
}

float ara_evaporation_linear(float old_pheromone_value, uint64_t milliseconds_since_last_evaporation)
{
    if (milliseconds_since_last_evaporation == 0) {
        return old_pheromone_value;
    } else {
        float multiplicator = ((float)milliseconds_since_last_evaporation) / ((float) time_interval);
        /**
         *  this corresponds to f(x) = -mx + b or f(multiplicator) = -1 *
         *  evaporation_factor * multiplicator + old_pheromone_value
         */
        float new_pheromone_value= ((-1) * evaporation_factor * multiplicator) + old_pheromone_value;

        if (new_pheromone_value < threshold) {
            new_pheromone_value = 0;
        }

        return new_pheromone_value;
    }
}

float ara_evaporation_exponential(float old_pheromone_value, uint64_t milliseconds_since_last_evaporation)
{
    if (milliseconds_since_last_evaporation == 0) {
        return old_pheromone_value;
    } else {
        float multiplicator = ((float)milliseconds_since_last_evaporation) / ((float) time_interval);
        float new_pheromone_value = old_pheromone_value * pow(evaporation_factor, multiplicator);

        if (new_pheromone_value < threshold) {
            new_pheromone_value = 0;
        }

        return new_pheromone_value;
    }
}
