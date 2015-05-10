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
 * @file        forwarding.h
 * @brief       Forwarding functions of ARA 
 *
 * @author      Michael Frey <frey@informatik.hu-berlin.de>
 */
#include "forwarding.h"


static float ara_get_random_number(void);
static void ara_initialize_random_number_generator(void);

void ara_forwarding_init(void)
{
    ara_initialize_random_number_generator();
}

struct ara_next_hop_t* ara_stochastic_forwarding(struct ara_routing_entry_t *entry)
{
    if (entry->nextHopListSize > 1) {
        /* the index of the next hop */
        uint8_t node_index = 0;
        /* the total sum of the pheromone values  */
        float pheromone_sum = .0;

        float* probabilities = (float*) malloc(sizeof(float) * entry->nextHopListSize);

        if (probabilities != NULL) {
            /* store the pheromone values temporary in the probablities array */
            for (uint8_t i = 0; i < entry->nextHopListSize; i++) {
                probabilities[i] = ara_get_pheromone_value(entry, i);
                /** store the raw pheromone values */
                pheromone_sum += probabilities[i];
            }

            /* compute the probablities array */
            for (uint8_t i = 0; i < entry->nextHopListSize; i++) {
                probabilities[i] = probabilities[i]/pheromone_sum;
            }

            float* result = (float*) malloc(sizeof(float) * entry->nextHopListSize);

            if (result != NULL) {
                /* compute the cumulative sum */
                ara_cum_sum(probabilities, result, entry->nextHopListSize);

                /* get a random number between 1.0 and 1.0 */
                float random_number = ara_get_random_number();

                while (random_number > result[node_index]) {
                    node_index += 1;
                }

                free(result);
                free(probabilities);

                struct ara_next_hop_t *next_hop = ara_get_next_hop_entry(entry, node_index);
                return next_hop;
            }

            free(probabilities);
        }
    }

    return NULL;
}

static void ara_initialize_random_number_generator(void)
{
    timex_t now;
    vtimer_now(&now);
    srand(timex_uint64(now));
}

static float ara_get_random_number(void)
{
    /* the random number should between [0,1] */
    return rand()/RAND_MAX;
}
