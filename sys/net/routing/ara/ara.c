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
 * @file        ara.c
 * @brief       ARA routing protocol
 *
 * @author      Michael Frey <frey@informatik.hu-berlin.de>
 */

#include "ara.h"
#include "forwarding.h"

void ara_init(void)
{
    /* initialize the probablistic forwarding */
    ara_forwarding_init();
}
