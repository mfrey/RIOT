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
 * @file        constants.h
 * @brief       Constansts related to the ARA protocol
 *
 * @author      Michael Frey <frey@informatik.hu-berlin.de>
 */

#ifndef ARA_CONSTANTS_H_
#define ARA_CONSTANTS_H_

#ifdef __cplusplus
extern "C" {
#endif

enum ara_constants {
    ARA_MAX_ROUTING_ENTRIES = 256 /**< maximum number of routing table entries */
};

#ifdef __cplusplus
}
#endif

#endif /* ARA_CONSTANTS_H_ */
