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
 * @file        evaporation.h
 * @brief       Evaporation functions of ARA 
 *
 * @author      Michael Frey <frey@informatik.hu-berlin.de>
 */

#ifndef ARA_EVAPORATION_H_
#define ARA_EVAPORATION_H_


#ifdef __cplusplus
extern "C" {
#endif

/**
 * The function initializes variables required for the evaporation function(s).
 * The pheromones will be evaporated every x milliseconds (time * interval)
 * according to the following formula: 
 *
 *    new_pheromone_value = old_pheromone_value * factor
 *
 * @param[in] factor The evaporation factor which is used to evaporate the
 * pheromone values
 * @param[in] time_interval_millisecondes The parameter denotes the time 
 * interval in milliseconds at which the evaporation factor should be
 * applied
 * @param[in] threshold The threshold denotes the smallest allowed pheromone 
 * value before it is set to 0
 */
void ara_evaporation_init(float evaporation_factor, float threshold, unsigned int time_interval_millis);

/**
 * The method evaporates pheromones in a linear fashion. This means that
 * pheromones evaporate as follows:
 *  
 *  f(t)= -m * t + phi
 * 
 * Here, m denotes the evaporation factor, meaning how fast pheromones
 * evaporate and the corresponding point in time.
 *
 * @param[in] old_pheromone_value The pheromone value which is going to be
 * changed
 * @param[in] milliseconds_since_last_evaporation The time in milliseconds which
 * has passed since the function has been called last time
 *
 * @return The new pheromone value after the evaporation function has beend
 * applied.
 */
float ara_evaporation_linear(float old_pheromone_value, int milliseconds_since_last_evaporation);


float ara_evaporation_exponential(float old_pheromone_value, int milliseconds_since_last_evaporation);

#ifdef __cplusplus
}
#endif

#endif /* ARA_FORWARDING_H_*/
