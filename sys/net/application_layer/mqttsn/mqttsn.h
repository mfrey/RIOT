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
 * @file        mqttsn.h
 * @brief       Structs, enums, and functions for marshalling/unmarshalling MQTT-SN messages
 *
 * @author      Michael Frey <mail@mfrey.net>
 */


#ifndef MQTTSN_H_
#define MQTTSN_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "net/ipv6/addr.h"

#include "constants.h"

#ifdef __cpluslus
extern "C" {
#endif 

/**
 * @brief Defines the types (msg_type field) of  MQTT-SN messages.
 */
typedef enum {
    MQTTSN_TYPE_ADVERTISE       = 0x00,
    MQTTSN_TYPE_SEARCHGW        = 0x01,
    MQTTSN_TYPE_GWINFO          = 0x02,
    MQTTSN_TYPE_CONNECT         = 0x04,
    MQTTSN_TYPE_CONNACK         = 0x05,
    MQTTSN_TYPE_WILLTOPICREQ    = 0x06,
    MQTTSN_TYPE_WILLTOPIC       = 0x07,
    MQTTSN_TYPE_WILLMSGREQ      = 0x08,
    MQTTSN_TYPE_WILLMSG         = 0x09,
    MQTTSN_TYPE_REGISTER        = 0x0A,
    MQTTSN_TYPE_REGACK          = 0x0B,
    MQTTSN_TYPE_PUBLISH         = 0x0C,
    MQTTSN_TYPE_PUBACK          = 0x0D,
    MQTTSN_TYPE_PUBCOMP         = 0x0E,
    MQTTSN_TYPE_PUBREC          = 0x0F,
    MQTTSN_TYPE_PUBREL          = 0x10,
    MQTTSN_TYPE_SUBSCRIBE       = 0x12,
    MQTTSN_TYPE_SUBACK          = 0x13,
    MQTTSN_TYPE_UNSUBSCRIBE     = 0x14,
    MQTTSN_TYPE_UNSUBACK        = 0x15,
    MQTTSN_TYPE_PINGREQ         = 0x16,
    MQTTSN_TYPE_PINGRESP        = 0x17,
    MQTTSN_TYPE_DISCONNECT      = 0x18,
    MQTTSN_TYPE_WILLTOPICUPD    = 0x1A,
    MQTTSN_TYPE_WILLTOPICRESP   = 0x1B,
    MQTTSN_TYPE_WILLMSGUPD      = 0x1C,
    MQTTSN_TYPE_WILLMSGRESP     = 0x1D,
    MQTTSN_TYPE_ENCMSG          = 0xFE
} mqttsn_msg_t;

typedef enum {
    MQTTSN_RETURN_CODE_ACCEPTED               = 0x00,
    MQTTSN_RETURN_CODE_REJECTED_CONGESTION    = 0x01,
    MQTTSN_RETURN_CODE_REJECTED_INVALID_TOPIC = 0x02,
    MQTTSN_RETURN_CODE_REJECTED_NOT_SUPPORTED = 0x03
} mqttsn_return_code;


typedef enum {
    MQTTSN_FLAG_RETAIN        = 0x1 << 4,
    MQTTSN_FLAG_WILL          = 0x1 << 3,
    MQTTSN_FLAG_CLEAN_SESSION = 0x1 << 2,
    MQTTSN_FLAG_DUP           = 0x1 << 7,
    MQTTSN_FLAG_QOS_0         = 0x0 << 5,
    MQTTSN_FLAG_QOS_1         = 0x1 << 5,
    MQTTSN_FLAG_QOS_2         = 0x2 << 5,
    MQTTSN_FLAG_QOS_N1        = 0x3 << 5
} mqttsn_flag_t;

/**
 * Topic types define which type of information are included in a SUBSCRIBE
 * message.
 */
typedef enum {
    MQTTSN_TOPIC_TYPE_NAME = 0x00,       /**< topic name */
    MQTTSN_TOPIC_TYPE_PRE_DEFINED= 0x01, /**< pre-defined topic id */
    MQTTSN_TOPIC_TYPE_SHORT = 0x10,      /**< short topic name */
    MQTTSN_TOPIC_TYPE_RESERVED = 0x11    /**< reserved */
} mqttsn_topic_t;

/**
 * The MQTT-SN message header which is two or up to four octet large 
 * and consists of a length and message type field. The length
 * field is variable in size (1 to 3). This header only defines
 * a length field of one octet. Hence, larger message headers
 * have to specified seperatly.
 */
typedef struct __attribute__((packed)) {
    uint8_t length;    /**< length of the message */
    uint8_t msg_type;  /**< the type of the message */
} mqttsn_msg_header_t;

/**
 * The DISCONNECT message is sent by a client to indicate that it wants
 * to close the connection. The gateway will acknowledge the receipt of
 * the message by returning a DISCONNECT message to the client. A 
 * DISCONNECT message might also be sent by a gateway or server in case
 * of an error. If the duration field is set by a client, the field 
 * indicates when the clients intents to go to the 'asleep' state. The 
 * gateway replies as well without a duration field.
 */
typedef struct __attribute__((packed)) {
    mqttsn_msg_header_t header;
    uint16_t duration;
} mqttsn_msg_disconnect_t;

/**
 * The ADVERTISE message is periodically broadcasted by a gateway to 
 * advertise its presence. 
 */
typedef struct __attribute__((packed)) {
    mqttsn_msg_header_t header;
    uint8_t gw_id;              /**< the gateway id of the gateway */
    uint16_t duration;          /**< the time interval until the next broadcast of the gateway */
} mqttsn_msg_advertise_t;

/**
 * The SEARCHGW message is sent by a client in order to find a gateway.
 */
typedef struct __attribute__((packed)) {
    mqttsn_msg_header_t header;
    uint16_t radius;            /**< the radius the message is broadcasted */
} mqttsn_msg_searchgw_t;

/**
 * The GWINFO message is sent in response to SEARCHGW message. If it is sent by a gateway,
 * it only contains the gateway id, otherwise, if sent by a client it also
 * contains the address of the gateway.
 */
typedef struct __attribute__((packed)) {
    mqttsn_msg_header_t header;
    uint8_t gw_id;              /**< the gateway id of the gateway */
} mqttsn_msg_gwinfo_t;

typedef struct __attribute__((packed)) {
    mqttsn_msg_header_t header;
    uint8_t flags;
    uint8_t protocol_identifier;
    uint16_t duration;
} mqttsn_msg_connect_t;

typedef struct __attribute__((packed)) {
    mqttsn_msg_header_t header;
    uint16_t topic_id;
    uint16_t msg_id;
} mqttsn_msg_register_t;

/**
 *
 */
typedef struct __attribute__((packed)) {
    mqttsn_msg_header_t header;
    uint16_t topic_id;
    uint16_t msg_id;
    uint8_t return_code;
} mqttsn_msg_register_acknowledgement_t;

typedef struct __attribute__((packed)) {
    mqttsn_msg_header_t header;
    uint8_t flags;
    uint16_t msg_id;
} mqttsn_msg_subscribe_t;

typedef struct __attribute__((packed)) {
    mqttsn_msg_header_t header;
    uint8_t flags;
    uint16_t topic_id;
    uint16_t msg_id;
} mqttsn_msg_publish_t;

typedef struct __attribute__((packed)) {
    mqttsn_msg_header_t header;
    /** control information exchanged between forwarder/gateway */
    uint8_t ctrl;           
    /** the wireless node identifier of this device */
    uint8_t wireless_node_id[MQTTSN_MAX_WIRELESS_NODE_ID_LENGTH];  
    /** the actual wrapped MQTT-SN message */
    char msg[MQTTSN_MAX_PACKET_LENGTH];
} mqttsn_msg_forward_encapsulation_t;

/**
 * The CONNACK message is sent by the server in response to a
 * connection request from a client. 
 */
typedef struct __attribute__((packed)) {
    mqttsn_msg_header_t header;
    uint8_t return_code; /**< */
} mqttsn_msg_connack;

/**
 * The WILLTOPICREQ message is sent by the gateway to request a
 * client for sending the Will topic.
 */
typedef struct __attribute__((packed)) {
    uint8_t length;      /**< */
    uint8_t msg_type;    /**< */
} mqttsn_msg_willtopicreq_t;

/**
 * The WILLTOPIC message is sent by a client as response to the 
 * WILLTOPICREQ message for transferring its WILL topic name to
 * the gateway.
 */
typedef struct __attribute__((packed)) {
    uint8_t length;      /**< */
    uint8_t msg_type;    /**< */
    uint8_t flags;       /**< */
} mqttsn_msg_willtopic_t;


void mqttsn_init(ipv6_addr_t src, uint16_t src_port, ipv6_addr_t dest, uint16_t dest_port, bool enable_forward_encapsulation);

/**
 * Sends a CONNECTION message in order to set up a connection.
 *
 * @param[in] client_identifier The identifier of this client.
 * @param[in] client_identifier_length The length of the client identifier
 * @param[in] duration The value of the keep alive timer
 * @param[in] clean_session Flag indicating if this is going to be a clean
 * session (as in MQTT)
 */
void mqttsn_connect(const char* client_identifier, size_t client_identifier_length, uint16_t duration, bool clean_session);

/**
 * Sends a DISCONNECT message in order to indicate that the client wants to
 * disconnect.
 */    
void mqttsn_disconnect(void);

/**
 * Sends a PINGREQ message in order to show if a gateway or client is still
 * alive.
 */    
void mqttsn_ping_request(void);

/**
 * Sends a SEARCHGW message in order to find a gateway.
 */    
void mqttsn_search_gateway(void);

/**
 * Requests a topic id value for the given topic name (REGISTER message).
 *
 * @param[in] topic_name The name of the topic
 * @param[in] topic_length The length of the topic
 * @param[in] topic_id The topic id value assigned by the gateway (only set by
 * gateways.
 */
void mqttsn_register(const char *topic_name, size_t topic_length, uint16_t topic_id);

/**
 * Sends a REGACK message as an acknowledgment to the receipt and processing of
 * a REGISTER message. 
 *
 * @param[in] topic_id The value which is used as topic id in the PUBLISH
 * messages.
 * @param[in] msg_id The same value as the one received in the REGISTER message.
 */
void mqttsn_register_acknowledgement(uint16_t topic_id, uint16_t msg_id);

/**
 * Subscribes to a given topic id (SUBSCRIBE message).
 *
 * @param[in] topic_identifier The topic identifier
 * @param[in] qos The QoS level of this topic
 */
void mqttsn_subscribe_topic_id(uint16_t topic_identifier, int8_t qos);

/**
 * Subscribes to a given topic name (SUBSCRIBE message).
 *
 * @param[in] topic_name The topic name a clients wants to subscribe to
 * @param[in] topic_length The size of the the topic name
 * @param[in] qos The QoS level of this topic
 */
void mqttsn_subscribe_topic_name(const char* topic_name, size_t topic_length,  int8_t qos);

/**
 * Publishes data for a given topic (PUBLISH message).
 *
 * @param[in] topic_identifier The topic identifier for which the data is
 * published.
 * @param[in] topic_type The type of the given topic_identifier
 * @param[in] data The published data
 * @param[in] payload_size The size of the published data
 * @param[in] qos The QoS level for this PUBLISH message
 * @param[in] retain The retain flag of this message
 */
void mqttsn_publish(uint16_t topic_identifier, uint8_t topic_type, const void* data, size_t payload_size, int8_t qos, uint8_t retain);

/**
 * Checks if a given node id is valid.
 *
 * @param[in] wireless_node_id The node id to check
 * @param[in] wireless_node_length 
 *
 * @return 0 if the identifier is a valid identifier, -1 if it is not, -2 if the
 * given length does not match and -3 if neither it is a valid id nor does the
 * length match.
 */
uint8_t mqttsn_check_wireless_node_id(uint8_t *wireless_node_id, uint8_t wireless_node_length);

/**
 * Handles the reception of a REGACK message.
 *
 * @param[in] packet The REGACK message received by the client.
 *
 * @return On success the topic identifier, upon failure TODO
 */
uint16_t mqttsn_handle_register_acknowledgement_msg(const mqttsn_msg_register_acknowledgement_t *packet);



/**
 * Performs various checks to verify that the packet is a well-formed MQTT-SN
 * message.
 *
 * @param[in] data The data received from the network stack
 * @param[in] length The size of the data
 *
 * @return 0 upon success, -1 if packet length is invalid, -2 if forward
 * encapsulation is enabled, but the packet is not encapsulated, -3 if there is
 * a size mismatch in the encapsulated packet, and -4 if the given size does 
 * not actually match the size of the packet.
 */
uint8_t mqttsn_validate(const void *data, size_t length);

/**
 * Handles ADVERTISE messages. The function simply adds gateways to its internal
 * gateway list. The actions specified in section 6.1 of the MQTT-SN
 * specification happen in the advertise.c file.
 *
 * TODO: address parameter
 *
 * @param[in] packet The received ADVERTISE message
 * @param[in] address The address of the gateway
 */
void mqttsn_handle_advertise_msg(const mqttsn_msg_advertise_t *packet, ipv6_addr_t *address);

/**
 * Sets the broadcast radius for SEARCHGW and GWINFO messages. In dense deployments, the
 * radius should be set to 1.
 *
 * @param[in] msg_radius The broadcast radius to set.
 */
void mqttsn_set_radius(uint8_t msg_radius);

void mqttsn_handle_searchgw_msg(const mqttsn_msg_searchgw_t *packet, ipv6_addr_t* source);

void mqttsn_handle_register_msg(const mqttsn_msg_register_t *packet);

void mqttsn_handle_msg(char *data, ipv6_addr_t *source);

uint8_t mqttsn_get_radius(void);

uint8_t mqttsn_get_type(void *data);

#ifdef __cpluslus
}
#endif 

#endif 
