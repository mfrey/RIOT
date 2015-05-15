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

#ifndef ARA_FORWARDING_H__
#define ARA_FORWARDING_H_

#include "helper.h"
#include "routingtable.h"

#include "timex.h"
#include "vtimer.h"

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief     The function initializes the forwarding functionality of ARA
 */
void ara_forwarding_init(void);

/**
 * @brief     Returns the next hop for a given destination 
 * @param[in] entry The routing table entry of a destination
 */
ara_next_hop_t* ara_stochastic_forwarding(ara_routing_entry_t *entry);


#ifdef __cplusplus
}
#endif

#endif /* ARA_FORWARDING_H_*/
