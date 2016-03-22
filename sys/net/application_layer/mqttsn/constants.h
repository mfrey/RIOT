#ifndef MQTTSN_CONSTANTS_H_
#define MQTTSN_CONSTANTS_H_

/**
 * The maxmimum packet length of a MQTTSN message in bytes.
 */
#define MQTTSN_MAX_PACKET_LENGTH (255)

/**
 * The maximum topic length. The constant '6' denotes the remaining 6 bytes
 * needed for the message.
 */
#define MQTTSN_MAX_TOPIC_LENGTH (MQTTSN_MAX_PACKET_LENGTH - 6)

/**
 * The maximum length of the wireless node id (which is used in the 
 * forwarder encapsulation).
 */
#define MQTTSN_MAX_WIRELESS_NODE_ID_LENGTH (252)

/**
 * The default value for ADVERTISE timers (in minutes). This refers
 * to the T_{ADV} value in the implementation notes of MQTT-SN.
 */
#define MQTTSN_TIMER_ADVERTISE (15)

/**
 * The default value for ADVERTISE counter. This refers
 * to the N_{ADV} value in the implementation notes of MQTT-SN.
 */
#define MQTTSN_COUNT_ADVERTISE (3)

/**
 * The default value for SEARCHGW timers (in seconds). This refers
 * to the T_{SEARCHGW} value in the implementation notes of MQTT-SN.
 */
#define MQTTSN_TIMER_SEARCHGW (5)

/**
 * The default value for GWINFO timers (in seconds). This refers
 * to the T_{GWINFO} value in the implementation notes of MQTT-SN.
 */
#define MQTTSN_TIMER_GWINFO (5)

/**
 * The default value for wait timer (in minutes). This refers
 * to the T_{WAIT} value in the implementation notes of MQTT-SN.
 */
#define MQTTSN_TIMER_TWAIT (5)

/**
 * The default value for retry timer (in seconds). This refers
 * to the T_{RETRY} value in the implementation notes of MQTT-SN.
 */
#define MQTTSN_TIMER_RETRY (10)

/**
 * The default value for number of retires counter. This refers
 * to the N_{ADV} value in the implementation notes of MQTT-SN.
 */
#define MQTTSN_COUNT_RETRY (4)


#endif 
