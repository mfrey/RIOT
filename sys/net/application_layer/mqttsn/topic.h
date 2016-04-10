#ifndef MQTTSN_TOPIC_H_
#define MQTTSN_TOPIC_H_


#include <stdint.h>
#include <stdbool.h>

#include "utlist.h"

#include "constants.h"

#ifdef __cpluslus
extern "C" {
#endif 

/**
 * A list element for a list of MQTT-SN topics
 */
typedef struct mqttsn_topic {
    /** the topic identifier */
    uint16_t topic_identifier;        
    /** the topic name */
    char topic_name[MQTTSN_MAX_TOPIC_LENGTH];
    /** pointer to the next list element */
    struct mqttsn_topic *next;
} mqttsn_topic_entry_t;

void mqttsn_topic_init(void);

bool mqttsn_topic_add(uint16_t topic_identifier, const char* topic_name);

/**
 * Checks if the given topic id is a valid topic id 
 *
 * @param[in] topic_identifier The topic id to check
 *
 * @return True if the topic id is valid, false otherwise 
 */
bool mqttsn_topic_valid_identifier(uint16_t topic_identifier);

/**
 * Checks if the given topic name is a valid topic name
 *
 * @param[in] topic_name The topic name to check
 *
 * @return True if the topic name is valid, false otherwise 
 */
bool mqttsn_topic_valid_name(const char *topic_name);

/**
 * Checks if a topic with a given topic identifier is already 
 * registered.
 *
 * @param[in] topic_id The topic to check
 *
 * @return True if there is a topic registered with the given id, false
 * otherwise.
 */
bool mqttsn_topic_contains(uint16_t topic_identifier);

/**
 * @brief Removes all topics from the map.
 */
void mqttsn_topic_clear(void);

const char* mqttsn_topic_get(uint16_t topic_identifier);

uint8_t mqttsn_will_topic_size(void);
uint8_t mqttsn_will_msg_size(void);

#ifdef __cpluslus
}
#endif 

#endif 
