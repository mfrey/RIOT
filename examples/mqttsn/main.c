/*
 * Copyright (C) 2015 Michael Frey
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       blafasel 
 *
 * @author      Michael Frey <mail@mfrey.net>
 *
 * @}
 */

#include <stdio.h>

#include "msg.h"
#include "net/mqttsn/mqttsn.h"
#include "net/mqttsn/commands.h"

#include "net/ipv6/addr.h"
#include "net/gnrc/netif.h"
#include "net/gnrc/ipv6/netif.h"

ipv6_addr_t* get_address(void)
{
    /** ipv6 address to use */
    ipv6_addr_t *result;
    /** registered network interfaces */
    kernel_pid_t interfaces[GNRC_NETIF_NUMOF];
    /* get the number of addresses stored in the IPv6 interfaces */
    size_t number_of_addresses = gnrc_netif_get(interfaces);

    if (number_of_addresses > 0) {
        /** always take the first interface */
        gnrc_ipv6_netif_t *entry = gnrc_ipv6_netif_get(interfaces[0]);
        for (int i = 0; i < GNRC_IPV6_NETIF_ADDR_NUMOF; i++) {
            if ((ipv6_addr_is_link_local(&entry->addrs[i].addr)) && !(entry->addrs[i].flags & GNRC_IPV6_NETIF_ADDR_FLAGS_NON_UNICAST)) {
                result = &entry->addrs[i].addr;
            }
        }
    }

    return result;
}

int main(void)
{
    puts("mqtt-sn example");

    ipv6_addr_t *source = get_address();

    msg_t m;
    m.type = MQTTSN_CMD_STATUS;
    kernel_pid_t pid = mqttsn_init(*source, 1883, false, 0);
    msg_send_receive(&m, &m, pid);  

    /* main thread exits */
    return 0;
}
