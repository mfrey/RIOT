#include "mqttsn.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include <byteorder.h>


#include "topic.h"
#include "communication.h"

/** pid for the mqtt-sn process  */
kernel_pid_t mqttsn_pid = KERNEL_PID_UNDEF;
/** message queue for the mqtt-sn process */
static char _mqttsn_stack[MQTTSN_STACK_SIZE];
/** sequence number for mqtt-sn messages */
static uint16_t message_id = 1;


static uint8_t mqttsn_get_qos_flag(int8_t qos) 
{
    switch (qos) {
        case 0:
            return MQTTSN_FLAG_QOS_0;
        case 1:
            return MQTTSN_FLAG_QOS_1;
        case 2:
            return MQTTSN_FLAG_QOS_2;
        case -1:
            return MQTTSN_FLAG_QOS_N1;
        default:
            return 0;
    }
}

static void *mqttsn_event_loop(void *args) 
{
    msg_t message; 
    msg_t message_queue[MQTTSN_MSG_QUEUE_SIZE];

    /** avoid the warning about the unused variable*/
    (void)args;

    if(msg_init_queue(message_queue, MQTTSN_MSG_QUEUE_SIZE) == -1){
#if ENABLE_DEBUG 
        printf("could not initialize message queue!\n");
#endif 
        return NULL;
    }

    while (1) {
        msg_receive(&message);

        switch (message.type) {
            default:
                break;
        }
    }

    return NULL;
}

void mqttsn_init(ipv6_addr_t src, uint16_t src_port, ipv6_addr_t dest, uint16_t dest_port, bool enable_forward_encapsulation) 
{
    mqttsn_communication_init(src, src_port, dest, dest_port, enable_forward_encapsulation);
    mqttsn_send = mqttsn_communication_send_udp;
    mqttsn_receive = mqttsn_communication_receive_udp;

    /* initialize the mqtt-sn thread */
    if (mqttsn_pid == KERNEL_PID_UNDEF) {
        mqttsn_pid = thread_create(_mqttsn_stack, sizeof(_mqttsn_stack), MQTTSN_PRIO,
                THREAD_CREATE_STACKTEST, mqttsn_event_loop, NULL, "mqtt-sn");
    }
}


void mqttsn_connect(const char* client_identifier, size_t client_identifier_length, uint16_t duration, bool clean_session) {
    /** 
     * check if client identifier is not set or larger than 23 bytes (the maximum
     * length) 
     */
    if (!client_identifier || client_identifier_length > 23) {
#if ENABLE_DEBUG 
        printf("the client identifier is not set or too large!\n");
#endif 
        return;
    } 

    /** actual length of the packet */
    size_t length = 0x06 + client_identifier_length;
    /** raw packet */ 
    char data[length];
    /** wrap raw packet around connect packet structure */
    mqttsn_msg_connect_t* connect_packet = (mqttsn_msg_connect_t*)data;
    memset(&connect_packet, 0, sizeof(connect_packet));

    /** set the parameters of the packet */
    connect_packet->header.length = 0x06 + client_identifier_length;
    connect_packet->header.msg_type = MQTTSN_TYPE_CONNECT;
    connect_packet->flags = 0;

    if (clean_session) {
        connect_packet->flags = MQTTSN_FLAG_CLEAN_SESSION;
    }

    connect_packet->protocol_identifier = 0x01;
    connect_packet->duration = HTONS(duration);

    size_t size = strnlen(client_identifier, 23); 

    if (size != client_identifier_length) {
#if ENABLE_DEBUG 
        printf("there is a mismatch in the size of the client identifier. got: %d but expected: %d !\n", size, client_identifier_length);
#endif 
        return;
    }
    
    memcpy((connect_packet+1), client_identifier, size);

    // TODO
    if (duration) {

    }

    /** send the packet */
    mqttsn_send(connect_packet);
}

void mqttsn_disconnect(void) {
    /** prepare the disconnect packet */
    mqttsn_msg_disconnect_t disconnect_packet;
    memset(&disconnect_packet, 0, sizeof(disconnect_packet));

    /** set the members */
    disconnect_packet.header.msg_type = MQTTSN_TYPE_DISCONNECT;
    disconnect_packet.header.length = 0x02;

    // TODO: what's about the duration

    /** send the packet */
    mqttsn_send(&disconnect_packet);
}

void mqttsn_ping_request(void) 
{
    char ping_request_packet[2];
    /** the length of the packet */
    ping_request_packet[0] = 2;
    /** the type of the packet */
    ping_request_packet[1] = MQTTSN_TYPE_PINGREQ;
    /** send the packet */
    mqttsn_send(&ping_request_packet);
}

void mqttsn_register(const char *topic_name, size_t topic_length, uint16_t topic_id) 
{
    if (!topic_name || (topic_length > MQTTSN_MAX_TOPIC_LENGTH)) {
#if ENABLE_DEBUG 
        printf("the topic name is not set or too large!\n");
#endif 
        return;
    }

    /** actual length of the packet */
    size_t length = 0x06 + topic_length;
    /** raw packet */ 
    char data[length];
    /** wrap raw packet around connect packet structure */
    mqttsn_msg_register_t* register_packet = (mqttsn_msg_register_t*)data;
    memset(register_packet, 0, sizeof(mqttsn_msg_register_t));

    register_packet->header.msg_type = MQTTSN_TYPE_REGISTER;
    /** topic id is always 0x000 for clients */
    register_packet->topic_id = topic_id;
    /** get a new sequence number */
    register_packet->msg_id = HTONS(message_id++);
    /** set the size of the packet */
    register_packet->header.length = 0x06 + topic_length;

    size_t size = strnlen(topic_name, MQTTSN_MAX_TOPIC_LENGTH); 

    if (size != topic_length) {
#if ENABLE_DEBUG 
        printf("there is a mismatch in the size of the topic length. got: %d but expected: %d !\n", size, topic_length);
#endif 
        return;
    }

    /** set the topic */
    memcpy((register_packet+1), topic_name, size);
    /** send the packet */
    mqttsn_send(register_packet);
}

void mqttsn_register_acknowledgement(uint16_t topic_id, uint16_t msg_id) 
{
    mqttsn_msg_register_acknowledgement_t register_acknowledgement_packet;
    memset(&register_acknowledgement_packet, 0, sizeof(mqttsn_msg_register_acknowledgement_t));

    register_acknowledgement_packet.header.msg_type = MQTTSN_TYPE_REGACK;
    register_acknowledgement_packet.topic_id = HTONS(topic_id);
    register_acknowledgement_packet.msg_id = HTONS(msg_id);
    // TODO: check when we're going to reject a REGISTER message
    register_acknowledgement_packet.return_code = MQTTSN_RETURN_CODE_ACCEPTED;
    register_acknowledgement_packet.header.length = 0x07;

    mqttsn_send(&register_acknowledgement_packet);
}

void mqttsn_subscribe_topic_id(uint16_t topic_identifier, int8_t qos) 
{
    /** actual length of the packet */
    size_t length = 0x06 + sizeof(topic_identifier);
    /** raw packet */ 
    char data[length];
    mqttsn_msg_subscribe_t *subscribe_topic_packet = (mqttsn_msg_subscribe_t*)data;
    memset(subscribe_topic_packet, 0, sizeof(mqttsn_msg_subscribe_t));

    subscribe_topic_packet->header.msg_type = MQTTSN_TYPE_SUBSCRIBE;
    subscribe_topic_packet->flags = 0x00;
    subscribe_topic_packet->flags += mqttsn_get_qos_flag(qos);
    subscribe_topic_packet->flags += MQTTSN_TOPIC_TYPE_PRE_DEFINED;
    subscribe_topic_packet->msg_id = HTONS(message_id++);
    subscribe_topic_packet->header.length = 0x05 + 2;

    topic_identifier = HTONS(topic_identifier);

    /** set the topic id */
    memcpy((subscribe_topic_packet+1), &topic_identifier, sizeof(topic_identifier));

    mqttsn_send(subscribe_topic_packet);
}

void mqttsn_subscribe_topic_name(const char* topic_name, size_t topic_length, int8_t qos) 
{
    size_t size = strnlen(topic_name, MQTTSN_MAX_TOPIC_LENGTH);

    if (size != topic_length) {
#if ENABLE_DEBUG 
        printf("there is a mismatch in the size of the topic length. got: %d but expected: %d !\n", size, topic_length);
#endif 
        return;
    }

    /** actual length of the packet */
    size_t length = 0x06 + size;
    /** raw packet */ 
    char data[length];
    mqttsn_msg_subscribe_t *subscribe_topic_packet = (mqttsn_msg_subscribe_t*)data;
    memset(subscribe_topic_packet, 0, sizeof(mqttsn_msg_subscribe_t));

    subscribe_topic_packet->header.msg_type = MQTTSN_TYPE_SUBSCRIBE;
    subscribe_topic_packet->msg_id = HTONS(message_id++);
    subscribe_topic_packet->header.length = 0x05 + size;

    subscribe_topic_packet->flags = 0x00;
    subscribe_topic_packet->flags += mqttsn_get_qos_flag(qos);
    if (size == 2) {
        subscribe_topic_packet->flags += MQTTSN_TOPIC_TYPE_SHORT;
    } else {
        subscribe_topic_packet->flags += MQTTSN_TOPIC_TYPE_NAME;
    }
        
    /** set the topic id */
    memcpy((subscribe_topic_packet+1), topic_name, size);

    mqttsn_send(subscribe_topic_packet);
}

void mqttsn_publish(uint16_t topic_identifier, uint8_t topic_type, const void* data, size_t payload_size, int8_t qos, uint8_t retain) 
{
    if (!data || (payload_size > (MQTTSN_MAX_PACKET_LENGTH-7))) {
#if ENABLE_DEBUG 
        printf("the payload is not set or too large!\n");
#endif 
        return;
    }

    size_t size = strnlen(data, MQTTSN_MAX_PACKET_LENGTH-7);

    if (size != payload_size) {
#if ENABLE_DEBUG 
        printf("there is a mismatch in the size of the topic length. got: %d but expected: %d !\n", size, topic_length);
#endif 
        return;
    }

    char packet[size+7];
    mqttsn_msg_publish_t *publish_packet = (mqttsn_msg_publish_t*)packet;

    publish_packet->header.msg_type = MQTTSN_TYPE_PUBLISH;

    publish_packet->flags = 0x00;
    publish_packet->flags +=  mqttsn_get_qos_flag(qos);
    if (retain) {
        publish_packet->flags += MQTTSN_FLAG_RETAIN;
    }

    publish_packet->flags += (topic_type & 0x3);
    publish_packet->msg_id = HTONS(message_id++);
    publish_packet->topic_id = HTONS(topic_identifier);
    publish_packet->header.length = 0x07 + size;

    memcpy((publish_packet+1), data, size);

    mqttsn_send(publish_packet);
}


void mqttsn_handle_register_msg(const mqttsn_msg_register_t *packet) 
{
    uint8_t msg_id = NTOHS(packet->msg_id);
    uint16_t topic_identifier = NTOHS(packet->topic_id);
    // TODO:
    //const char* topic_name = packet->topic_name;
    const char* topic_name = "TODO";

    /**
     * TODO: if a gateway can't accept the registration it sends back a register
     * message _and_ a registration ack. I'm a bit puzzled what kind of 
     * topic name is set by the gateway. We should somehow handle this here? 
     */

    if(!mqttsn_topic_add(topic_identifier, topic_name)) {
#if ENABLE_DEBUG 
        printf("could not add topic\n");
#endif 
        // TODO: do we need to send a negativ ack?
    }

    mqttsn_register_acknowledgement(topic_identifier, msg_id); 
}

uint16_t mqttsn_handle_register_acknowledgement_msg(const mqttsn_msg_register_acknowledgement_t *packet) 
{
    uint16_t topic_id = NTOHS(packet->topic_id);
    uint16_t msg_id = NTOHS(packet->msg_id);
    uint8_t return_code = NTOHS(packet->return_code);

    /** check if the message id matches */
    if (msg_id != message_id -1) {

    }

    /** */

    if (return_code == MQTTSN_RETURN_CODE_REJECTED_CONGESTION) {
        // TODO: we have to wait T_WAIT before we try to re-register
    }

    return topic_id;
}

/*
uint8_t mqttsn_validate() 
{

}
*/

void mqttsn_handle_disconnect_msg(const mqttsn_msg_disconnect_t *packet) 
{
    if (packet) {
        if (packet->header.length == 4) {

        }
    } else {

    }
}
