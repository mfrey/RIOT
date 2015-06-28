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
 * @file        packet.c
 * @brief       ARA packets and helper functions.
 *
 * @author      Michael Frey <frey@informatik.hu-berlin.de>
 */

#include "packet.h" 

const char* ara_packet_get_type_as_string(char type)
{
    switch (type) {
        case DATA: return "DATA\0";
        case FANT: return "FANT\0";
        case BANT: return "BANT\0";
        default: return "UNKNOWN\0";
    }
}
