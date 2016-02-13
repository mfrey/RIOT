/*
 * Copyright (C) 2015 Michael Frey <frey@informatik.hu-berlin.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */
#include "tests-ara.h"
#include "tests-ara-util.h"

#include "net/routing/ara/routingtable.h"

static void test_ara_routing_table_add_entry(void)
{
    ara_routing_entry_t entry, another_entry; 
    memset(&entry, 0, sizeof(ara_routing_entry_t));
    memset(&another_entry, 0, sizeof(ara_routing_entry_t));

    ipv6_addr_t address = { {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
        }
    };

    ipv6_addr_t another_address = { {
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07
        }
    };

    /* add first entry */
    entry.destination = &address;
    ara_routing_table_add_entry(&entry);

    TEST_ASSERT_EQUAL_INT(1, ara_routing_table_size()); 
    TEST_ASSERT_EQUAL_INT(true, ara_routing_table_entry_exists(&address)); 

    /* add another entry with a different address */
    another_entry.destination = &another_address;
    ara_routing_table_add_entry(&another_entry);

    TEST_ASSERT_EQUAL_INT(2, ara_routing_table_size()); 
    TEST_ASSERT_EQUAL_INT(true, ara_routing_table_entry_exists(&another_address)); 

    ara_routing_table_del_entry(&another_entry);

    TEST_ASSERT_EQUAL_INT(1, ara_routing_table_size()); 
    TEST_ASSERT_EQUAL_INT(false, ara_routing_table_entry_exists(&another_address)); 

    ara_routing_table_del_entry(&entry);
}

static void test_ara_routing_table_get_entry(void)
{
    ara_routing_entry_t entry; 
    memset(&entry, 0, sizeof(ara_routing_entry_t));

    //printf("size is: %d\n", ara_routing_table_size());

    ipv6_addr_t address = { {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0xff
        }
    };

    /* the entry should not be in the routing table */
    TEST_ASSERT_EQUAL_INT(true, ara_routing_table_get_entry(&address) == NULL); 

    entry.destination = &address;
    /* add the entry to the routing table */
    ara_routing_table_add_entry(&entry);

    /* the entry should be in the routing table */
    TEST_ASSERT_EQUAL_INT(true, ara_routing_table_get_entry(&address) != NULL); 

    TEST_ASSERT_EQUAL_INT(1, ara_routing_table_size()); 

    ara_routing_table_del_entry(&entry);
}

static void test_ara_routing_table_add_next_hop(void)
{
    ara_routing_entry_t entry; 
    memset(&entry, 0, sizeof(ara_routing_entry_t));

    ipv6_addr_t address = { {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
        }
    };

    ipv6_addr_t next_hop_address = { {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
        }
    };

    entry.destination = &address;
    ara_routing_table_add_entry(&entry);

    ara_next_hop_t next_hop;
    next_hop.phi = 42.;
    next_hop.ttl = 23;
    next_hop.credit = 1.;
    next_hop.address =  &next_hop_address;

    ara_routing_table_add_next_hop(&entry, &next_hop);

    TEST_ASSERT_EQUAL_INT(1, entry.size); 
    TEST_ASSERT_EQUAL_INT(1, ara_routing_table_size()); 

    ara_routing_table_del_entry(&entry);
}

static void test_ara_routing_table_del_next_hops(void)
{
    ara_routing_entry_t entry; 
    memset(&entry, 0, sizeof(ara_routing_entry_t));

    ipv6_addr_t address = { {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
        }
    };

    ipv6_addr_t next_hop_address = { {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
        }
    };

    entry.destination = &address;
    ara_routing_table_add_entry(&entry);

    ara_next_hop_t next_hop;
    next_hop.phi = 42.;
    next_hop.ttl = 23;
    next_hop.credit = 1.;
    next_hop.address =  &next_hop_address;

    ara_routing_table_add_next_hop(&entry, &next_hop);

    TEST_ASSERT_EQUAL_INT(1, entry.size); 
    ara_routing_table_del_next_hops(&entry);

    TEST_ASSERT_EQUAL_INT(0, entry.size); 
    ara_routing_table_del_entry(&entry);
}

static void test_ara_routing_table_is_deliverable(void)
{
    ara_next_hop_t next_hop;

    ara_routing_entry_t entry; 
    memset(&entry, 0, sizeof(ara_routing_entry_t));

    ipv6_addr_t address = { {
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07
        }
    };


    ipv6_addr_t next_hop_address = { {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
        }
    };

    next_hop.address = &next_hop_address;
    entry.destination = &address;

    /* no entry and hence not deliverable */
    TEST_ASSERT_EQUAL_INT(false, ara_routing_table_is_deliverable(&address)); 

    /* we've added the entry, but no next hops */
    ara_routing_table_add_entry(&entry);
    TEST_ASSERT_EQUAL_INT(false, ara_routing_table_is_deliverable(&address)); 

    ara_routing_table_add_next_hop(&entry, &next_hop);

    /* we've added an next hop, so packets to that destination should be deliverbale */
    TEST_ASSERT_EQUAL_INT(true, ara_routing_table_is_deliverable(&address)); 

    ara_routing_table_del_entry(&entry);
}

static void test_ara_routing_table_next_hop_compare(void)
{
    ipv6_addr_t first_next_hop_address = { {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
        }
    };

    ipv6_addr_t second_next_hop_address = { {
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07
        }
    };

    ara_next_hop_t first_next_hop;
    first_next_hop.address = &first_next_hop_address;

    ara_next_hop_t second_next_hop;
    second_next_hop.address = &second_next_hop_address;

    TEST_ASSERT_EQUAL_INT(0, ara_routing_table_next_hop_compare(&first_next_hop, &first_next_hop)); 
    TEST_ASSERT_EQUAL_INT(0, ara_routing_table_next_hop_compare(&second_next_hop, &second_next_hop)); 

    TEST_ASSERT_EQUAL_INT(-1, ara_routing_table_next_hop_compare(&first_next_hop, &second_next_hop)); 
}

static void test_ara_routing_table_update(void)
{
    ara_next_hop_t next_hop;

    ara_routing_entry_t entry; 
    ara_routing_entry_t* temp_entry; 
    memset(&entry, 0, sizeof(ara_routing_entry_t));

    ipv6_addr_t address = { {
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07
        }
    };

    ipv6_addr_t next_hop_address = { {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
        }
    };

    next_hop.address = &next_hop_address;
    next_hop.phi = 42.;
    next_hop.ttl = 23;
    next_hop.credit = 1.;
    entry.destination = &address;

    ara_routing_table_add_entry(&entry);
    ara_routing_table_add_next_hop(&entry, &next_hop);

    /** fetch the entry and check the phi value */
    temp_entry = ara_routing_table_get_entry(&address);
    /** phi should be the old value */
    TEST_ASSERT_EQUAL_INT(42, temp_entry->next_hops->phi); 
    /** let's update the entry to a new value */
    ara_routing_table_update(&address, &next_hop_address, 23);

    /** again, fetch the entry and check the phi value */
    temp_entry = ara_routing_table_get_entry(&address);
    /** phi should be the new value */
    TEST_ASSERT_EQUAL_INT(23, temp_entry->next_hops->phi); 

    ara_routing_table_del_entry(&entry);
}


Test *tests_ara_routingtable_tests(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture(test_ara_routing_table_add_entry),
        new_TestFixture(test_ara_routing_table_get_entry),
        new_TestFixture(test_ara_routing_table_update),
        new_TestFixture(test_ara_routing_table_add_next_hop),
        new_TestFixture(test_ara_routing_table_del_next_hops),
        new_TestFixture(test_ara_routing_table_is_deliverable),
        new_TestFixture(test_ara_routing_table_next_hop_compare)
    };

    EMB_UNIT_TESTCALLER(ara_routingtable_tests, NULL, NULL, fixtures);

    return (Test *)&ara_routingtable_tests;
}
