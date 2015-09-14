/*
 *
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
 * @file        reinforcement.c
 * @brief       Reinforcement functions of ARA 
 *
 * @author      Michael Frey <frey@informatik.hu-berlin.de>
 */

#include "reinforcement.h"

static float delta = 0.;

void ara_reinforcement_init(float delta_phi)
{
    delta = delta_phi;
}

float ara_reinforcement_linear_compute(float old_pheromone_value)
{
    return old_pheromone_value + delta;
}
