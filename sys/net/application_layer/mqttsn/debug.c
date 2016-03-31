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
 * @file        debug.c
 * @brief       Helper functions to debug mqtt-sn 
 *
 * @author      Michael Frey <mail@mfrey.net>
 */

#include "debug.h" 

#include "mqttsn.h"

const char* mqttsn_debug_return_code_to_string(uint8_t return_code)
{
    switch(return_code) {
        case MQTTSN_RETURN_CODE_ACCEPTED:                   return "Accepted";           
        case MQTTSN_RETURN_CODE_REJECTED_CONGESTION:        return "Reected: congestion";
        case MQTTSN_RETURN_CODE_REJECTED_INVALID_TOPIC:     return "Rejected: invalid topic ID";
        case MQTTSN_RETURN_CODE_REJECTED_NOT_SUPPORTED:     return "Rejected: not supported";
        default:                                            return "reserved";
    }
}

const char* mqttsn_debug_msg_type_to_string(uint8_t msg_type) 
{
    switch(msg_type) {
        case MQTTSN_TYPE_ADVERTISE:       return "ADVERTISE";
        case MQTTSN_TYPE_SEARCHGW:        return "SEARCHGW";
        case MQTTSN_TYPE_GWINFO:          return "GWINFO";
        case MQTTSN_TYPE_CONNECT:         return "CONECT";
        case MQTTSN_TYPE_CONNACK:         return "CONACK";
        case MQTTSN_TYPE_WILLTOPICREQ:    return "WILLTOPICREQ";
        case MQTTSN_TYPE_WILLTOPIC:       return "WILLTOPIC";
        case MQTTSN_TYPE_WILLMSGREQ:      return "WILLMSGREQ";
        case MQTTSN_TYPE_WILLMSG:         return "WILLMSG";
        case MQTTSN_TYPE_REGISTER:        return "REGISTER";
        case MQTTSN_TYPE_REGACK:          return "REGACK";
        case MQTTSN_TYPE_PUBLISH:         return "PUBLISH";
        case MQTTSN_TYPE_PUBACK:          return "PUBACK";
        case MQTTSN_TYPE_PUBCOMP:         return "PUBCOMP";
        case MQTTSN_TYPE_PUBREC:          return "PUBREC";
        case MQTTSN_TYPE_PUBREL:          return "PUBREL";
        case MQTTSN_TYPE_SUBSCRIBE:       return "SUBSCRIBE";
        case MQTTSN_TYPE_SUBACK:          return "SUBACK";
        case MQTTSN_TYPE_UNSUBSCRIBE:     return "UNSUBSCRIBE";
        case MQTTSN_TYPE_UNSUBACK:        return "UNSUBACK";
        case MQTTSN_TYPE_PINGREQ:         return "PINGREQ";
        case MQTTSN_TYPE_PINGRESP:        return "PINGRESP";
        case MQTTSN_TYPE_DISCONNECT:      return "DISCONNECT";
        case MQTTSN_TYPE_WILLTOPICUPD:    return "WILLTOPICUPD";
        case MQTTSN_TYPE_WILLTOPICRESP:   return "WILLTOPICRESP";
        case MQTTSN_TYPE_WILLMSGUPD:      return "WILMSGUPD";
        case MQTTSN_TYPE_WILLMSGRESP:     return "WILLMSGRESP";
        default:                          return "UNKNOWN MESSSAGE TYPE";
    }
}
