/*
 * Copyright (C) 2016 Michael Frey
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     mqtt-sn
 *
 * @{
 *
 * @file        commands.h
 * @brief       Defines for shell commands to interact with MQTT-SN
 *
 * @author  Michael Frey <mail@mfrey.net>
 */
#ifndef MQTTSN_COMMANDS_H_
#define MQTTSN_COMMANDS_H_

#define MQTTSN_CMD_STATUS         0x4201 /**< */
#define MQTTSN_CMD_TOPICS_GET     0x4202 /**< */
#define MQTTSN_CMD_TOPICS_SET     0x4203 /**< */
#define MQTTSN_CMD_SEARCHGW       0x4204 /**< sends a SEARCHGW message */
#define MQTTSN_CMD_PRINTGW        0x4205 /**< prints known MQTT-SN gateways */
#define MQTTSN_CMD_PRINTWILL      0x4206 /**< prints WILL topic and message */
#define MQTTSN_CMD_SET_WILL_MSG   0x4207 /**< sets WILL message */
#define MQTTSN_CMD_SET_WILL_TOPIC 0x4208 /**< sets WILL topic */
#define MQTTSN_CMD_SET_WILL_TOPIC 0x4208 /**< sets WILL topic */


#endif /* MQTTSN_COMMANDS_H_ */
