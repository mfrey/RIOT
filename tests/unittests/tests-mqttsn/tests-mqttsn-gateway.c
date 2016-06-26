/*
 * Copyright (C) 2016 Michael Frey <mail@mfrey.net>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include "tests-mqttsn.h"

#include "mqttsn/gateway.h"

static void teardown(void)
{
    mqttsn_gateway_clear();
}

static void test_mqttsn_gateway_size(void)
{
    uint8_t gw_id = 23;
    
    ipv6_addr_t address = { {                                                                                                                                                    
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,                                                                                                                      
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f                                                                                                                       
        }                                                                                                                                                                        
    };
    
    uint8_t duration = 42;

    TEST_ASSERT_EQUAL_INT(0, mqttsn_gateway_size());
    TEST_ASSERT_EQUAL_INT(true, mqttsn_gateway_add(gw_id, &address, duration));
    TEST_ASSERT_EQUAL_INT(1, mqttsn_gateway_size());
}


static void test_mqttsn_gateway_get_most_recent_entry(void)
{
    uint8_t gw_id = 23;
    
    ipv6_addr_t address = { {                                                                                                                                                    
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,                                                                                                                      
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f                                                                                                                       
        }                                                                                                                                                                        
    };
    
    uint8_t duration = 42;

    TEST_ASSERT_EQUAL_INT(0, mqttsn_gateway_size());
    mqttsn_gateway_entry_t *entry = mqttsn_gateway_get_most_recent_entry();
    TEST_ASSERT_NULL(entry);
    TEST_ASSERT_EQUAL_INT(true, mqttsn_gateway_add(gw_id, &address, duration));
    entry = mqttsn_gateway_get_most_recent_entry();
    TEST_ASSERT(entry != NULL);
}

static void test_mqttsn_gateway_contains(void)
{
    uint8_t gw_id = 23;
    
    ipv6_addr_t address = { {                                                                                                                                                    
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,                                                                                                                      
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f                                                                                                                       
        }                                                                                                                                                                        
    };
    
    uint8_t duration = 42;

    TEST_ASSERT_EQUAL_INT(0, mqttsn_gateway_size());
    TEST_ASSERT_EQUAL_INT(false, mqttsn_gateway_contains(&address));
    TEST_ASSERT_EQUAL_INT(true, mqttsn_gateway_add(gw_id, &address, duration));
    TEST_ASSERT_EQUAL_INT(true, mqttsn_gateway_contains(&address));
}

static void test_mqttsn_gateway_add(void)
{
    uint8_t gw_id = 23;
    
    ipv6_addr_t address = { {                                                                                                                                                    
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,                                                                                                                      
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f                                                                                                                       
        }                                                                                                                                                                        
    };
    
    uint8_t duration = 42;

    TEST_ASSERT_EQUAL_INT(0, mqttsn_gateway_size());
    TEST_ASSERT_EQUAL_INT(false, mqttsn_gateway_contains(&address));
    TEST_ASSERT_EQUAL_INT(true, mqttsn_gateway_add(gw_id, &address, duration));
    TEST_ASSERT_EQUAL_INT(true, mqttsn_gateway_contains(&address));
}

static void test_mqttsn_gateway_clear(void)
{
    uint8_t gw_id = 23;
    
    ipv6_addr_t address = { {                                                                                                                                                    
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,                                                                                                                      
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f                                                                                                                       
        }                                                                                                                                                                        
    };
    
    uint8_t duration = 42;

    TEST_ASSERT_EQUAL_INT(0, mqttsn_gateway_size());
    TEST_ASSERT_EQUAL_INT(true, mqttsn_gateway_add(gw_id, &address, duration));
    TEST_ASSERT_EQUAL_INT(1, mqttsn_gateway_size());
    mqttsn_gateway_clear();
    TEST_ASSERT_EQUAL_INT(0, mqttsn_gateway_size());
}

Test *tests_mqttsn_gateway_tests(void) 
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture(test_mqttsn_gateway_size),
        new_TestFixture(test_mqttsn_gateway_contains),
        new_TestFixture(test_mqttsn_gateway_get_most_recent_entry),
        new_TestFixture(test_mqttsn_gateway_clear),
        new_TestFixture(test_mqttsn_gateway_add)
    };

    EMB_UNIT_TESTCALLER(mqttsn_gateway_tests, NULL, teardown, fixtures);

    return (Test *)&mqttsn_gateway_tests;
}
