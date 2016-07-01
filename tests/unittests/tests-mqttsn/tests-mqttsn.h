/*
 * Copyright (C) 2016 Michael Frey <mail@mfrey.net>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @addtogroup  unittests
 * @{
 *
 * @file        tests-mqttsn.h
 * @brief       Unittests for the ``mqtt-sn`` module
 *
 * @author      Michael Frey <mail@mfrey.net>
 */
#ifndef TESTS_MQTTSN_H_
#define TESTS_MQTTSN_H_

#include "embUnit.h"

#include "mqttsn/mqttsn.h"
#include "mqttsn/constants.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   The entry point of this test suite.
 */
void tests_mqttsn(void);

void tests_mqttsn_dummy_send(void *data);

/**
 * @brief   Generates tests for MQTT-SN topics 
 *
 * @return  embUnit tests if successful, NULL if not.
 */
Test *tests_mqttsn_topic_tests(void);

Test *tests_mqttsn_will_tests(void);

/**
 * @brief   Generates tests for the device functions of MQTT-SN
 *
 * @return  embUnit tests if successful, NULL if not.
 */
Test *tests_mqttsn_device_tests(void);

/**
 * @brief   Generates tests for MQTT-SN messages
 *
 * @return  embUnit tests if successful, NULL if not.
 */
Test *tests_mqttsn_messages_tests(void);

Test *tests_mqttsn_gateway_tests(void);

/**
 * @brief   Generates tests for ARA
 *
 * @return  embUnit tests if successful, NULL if not.
 */
Test *tests_mqttsn_tests(void);

#ifdef __cplusplus
}
#endif

#endif /* TESTS_MQTTSN_H_ */
/** @} */
