/*
 * Copyright (C) 2016 Michael Frey <mail@mfrey.net>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     mqtt-sn
 * @{
 *
 * @file        data.c
 * @brief       Functions to handle data
 *
 * @author      Michael Frey <mail@mfrey.net>
 */

#include "data.h"

void (*mqttsn_data_process)(void *data, size_t size);

void mqttsn_data_init(void) 
{

}

void mqttsn_data_receive(void *data, size_t size) 
{
    mqttsn_data_process(data, size);
}

void mqttsn_set_data_process(void (*function)(void*, size_t)) 
{
    mqttsn_data_process = function;
}
