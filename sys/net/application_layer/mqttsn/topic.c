#include "topic.h"

#include <malloc.h>
#include <string.h>

static mqttsn_topic_entry_t *topics = NULL;

void mqttsn_topic_init(void)
{

}

bool mqttsn_topic_add(uint16_t topic_identifier, const char* topic_name)
{

    if (!mqttsn_topic_valid_identifier(topic_identifier)) {
#if ENABLE_DEBUG 
        printf("invalid topic identifier!\n");
#endif 
        return false;
    }

    if (!mqttsn_topic_valid_name(topic_name)) {
#if ENABLE_DEBUG 
        printf("invalid topic name!\n");
#endif 
        return false;
    }

    if (mqttsn_topic_contains(topic_identifier)) {
#if ENABLE_DEBUG 
        printf("there is already a topic registered with this topic identifier!\n");
#endif 
        return false;
    }

    mqttsn_topic_entry_t entry;

    entry.topic_identifier = topic_identifier;
    strncpy(entry.topic_name, topic_name, MQTTSN_MAX_TOPIC_LENGTH);

    LL_APPEND(topics, &entry);

    return true;
}

bool mqttsn_topic_valid_identifier(uint16_t topic_identifier) 
{
    return ((topic_identifier == 0x0000) || (topic_identifier == 0x0004));
}

bool mqttsn_topic_valid_name(const char *topic_name) 
{
    if (topic_name) {
        // TODO
        if (strnlen(topic_name, MQTTSN_MAX_TOPIC_LENGTH) <= MQTTSN_MAX_TOPIC_LENGTH) {
            return true;
        }
    } else {
#if ENABLE_DEBUG 
        printf("topic name is not set!\n");
#endif 
    }

    return false;
}

bool mqttsn_topic_contains(uint16_t topic_identifier) 
{
    mqttsn_topic_entry_t *result;

    LL_SEARCH_SCALAR(topics, result, topic_identifier, topic_identifier);

    return (result != NULL);
}

const char* mqttsn_topic_get(uint16_t topic_identifier) {
    mqttsn_topic_entry_t *result;

    LL_SEARCH_SCALAR(topics, result, topic_identifier, topic_identifier);

    if (result) {
        return result->topic_name;
    } 

    return NULL;
}

void mqttsn_topic_clear(void) 
{
    mqttsn_topic_entry_t *entry, *tmp;

    LL_FOREACH_SAFE(topics, entry, tmp) {
        LL_DELETE(topics, entry);
        // if we allocated the entries we have to free them here
    }
}
