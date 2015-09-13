/*
 * Copyright (C) 2015 Michael Frey <frey@informatik.hu-berlin.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */
#include "embUnit.h"

#include "tests-ara-helper.h"

#include "net/routing/ara/evaporation.h"

//#include "tests-ara-evaporation.h"

static void test_ara_evaporation_linear(void)
{
    float evaporationFactor = 0.25;
    float threshold = 0.75;
    unsigned int timeIntervalInMillis = 2000;

    ara_evaporation_init(evaporationFactor, threshold, timeIntervalInMillis);

    float pheromone = 1.5;

    /* nothing should happen if no time has passed */
    pheromone = ara_evaporation_linear(pheromone, 0);
    TEST_ASSERT(approximately_equal(1.5, pheromone, 0.00001) == true);

    /* first linear evaporation */
    pheromone = ara_evaporation_linear(pheromone, timeIntervalInMillis);
    TEST_ASSERT(approximately_equal(1.25, pheromone, 0.00001) == true);

    /* another linear evaporation */
    pheromone = ara_evaporation_linear(pheromone, timeIntervalInMillis);
    TEST_ASSERT(approximately_equal(1.00, pheromone, 0.00001) == true);


    /* check if the threshold sets the pheromone value to null */
    pheromone = ara_evaporation_linear(pheromone, timeIntervalInMillis);
    TEST_ASSERT(approximately_equal(0.75, pheromone, 0.00001) == true);
}

static void test_ara_evaporation_linear_with_zero_time_interval(void)
{
    float evaporationFactor = 0.25;
    float threshold = 0.75;

    ara_evaporation_init(evaporationFactor, threshold, 1000);

    float pheromone = 10;

    pheromone = ara_evaporation_linear(pheromone, 0);
    TEST_ASSERT(approximately_equal(10, pheromone, 0.00001) == true);
}

static void test_ara_evaporation_exponential(void)
{
    float evaporationFactor = 0.9;
    float threshold = 0.75;
    unsigned int timeIntervalInMillis = 2000;

    ara_evaporation_init(evaporationFactor, threshold, timeIntervalInMillis);

    float pheromone = 1;

    /* nothing should happen if no time has passed */
    pheromone = ara_evaporation_exponential(pheromone, timeIntervalInMillis);
    TEST_ASSERT(approximately_equal(1.0, pheromone, 0.00001) == true);

    /* first exponential evaporation */
    pheromone = ara_evaporation_exponential(pheromone, timeIntervalInMillis);
    TEST_ASSERT(approximately_equal(0.9, pheromone, 0.00001) == true);

    /* another exponential evaporation */
    pheromone = ara_evaporation_exponential(pheromone, timeIntervalInMillis);
    TEST_ASSERT(approximately_equal(0.81, pheromone, 0.00001) == true);

    /* check if the threshold sets the pheromone value to null */
    pheromone = ara_evaporation_exponential(pheromone, timeIntervalInMillis);
    TEST_ASSERT(approximately_equal(0.0, pheromone, 0.00001) == true);
}

static void test_ara_evaporation_exponential_with_zero_time_interval(void)
{
    float evaporationFactor = 0.9;
    float threshold = 0.75;
    unsigned int timeIntervalInMillis = 2000;

    ara_evaporation_init(evaporationFactor, threshold, timeIntervalInMillis);

    float pheromone = 10;

    pheromone = ara_evaporation_exponential(pheromone, 0);
    TEST_ASSERT(approximately_equal(10, pheromone, 0.00001) == true);
}

static void test_ara_evaporation_exponential_partial_evaporation(void)
{
    float evaporationFactor = 0.9;
    float threshold = 0.75;
    unsigned int timeIntervalInMillis = 1000;

    ara_evaporation_init(evaporationFactor, threshold, timeIntervalInMillis);

    float pheromone = 10;
    pheromone = ara_evaporation_exponential(pheromone, 500);
    TEST_ASSERT(approximately_equal(10 * pow(evaporationFactor, 0.5), pheromone, 0.00001) == true);
}

static void test_ara_evaporation_exponential_sequential_vs_continuous_evaporation(void) 
{
    float evaporationFactor = 0.7;
    float threshold = 1;
    unsigned int timeIntervalInMillis = 1000;

    ara_evaporation_init(evaporationFactor, threshold, timeIntervalInMillis);

    float originalPheromone = 123;
    float evaporationInOneStep = ara_evaporation_exponential(originalPheromone, 10 * timeIntervalInMillis);

    float evaporationInMultipleSteps = originalPheromone;
    for (int i = 0; i < 10; ++i) {
        evaporationInMultipleSteps = ara_evaporation_exponential(evaporationInMultipleSteps, timeIntervalInMillis);
    }

    TEST_ASSERT(approximately_equal(evaporationInOneStep, evaporationInMultipleSteps, 0.00001) == true);
}
