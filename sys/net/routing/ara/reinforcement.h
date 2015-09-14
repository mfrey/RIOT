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
 * @file        reinforcement.h
 * @brief       Reinforcement functions of ARA 
 *
 * @author      Michael Frey <frey@informatik.hu-berlin.de>
 */

#ifndef ARA_REINFORCEMENT_H_
#define ARA_REINFORCEMENT_H_

#ifdef __cplusplus
extern "C" {
#endif

void ara_reinforcement_init(float delta_phi);

float ara_reinforcement_linear_compute(float value);

#ifdef __cplusplus
}
#endif

#endif /* ARA_REINFORCEMENT_H_*/

