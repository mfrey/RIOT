/*
 * Copyright (C) 2015 Michael Frey <frey@informatik.hu-berlin.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */
#include "tests-ara.h"
#include "tests-ara-helper.h"

#include "net/routing/ara/routingtable.h"

static void test_ara_routing_table_add_entry(void)
{
    ara_routing_entry_t entry; 
    memset(&entry, 0, sizeof(ara_routing_entry_t));

    struct netaddr address = { {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
        }, AF_INET6, 128
    };

    struct netaddr another_address = { {
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07
        }, AF_INET6, 128
    };

    /* add first entry */
    entry.destination = &address;
    ara_routing_table_add_entry(&entry);

    TEST_ASSERT_EQUAL_INT(1, ara_routing_table_size()); 
    TEST_ASSERT_EQUAL_INT(true, ara_routing_table_entry_exists(&address)); 

    /* add another entry with a different address */
    entry.destination = &another_address;
    ara_routing_table_add_entry(&entry);

    TEST_ASSERT_EQUAL_INT(2, ara_routing_table_size()); 
    TEST_ASSERT_EQUAL_INT(true, ara_routing_table_entry_exists(&another_address)); 

    ara_routing_table_del_entry(&entry);

    TEST_ASSERT_EQUAL_INT(1, ara_routing_table_size()); 
    TEST_ASSERT_EQUAL_INT(false, ara_routing_table_entry_exists(&another_address)); 
}

Test *tests_ara_routingtable_tests(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture(test_ara_routing_table_add_entry)
    };

    EMB_UNIT_TESTCALLER(ara_routingtable_tests, NULL, NULL, fixtures);

    return (Test *)&ara_routingtable_tests;
}
