/*
 * Copyright (C) 2017 MSA Safety
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup pkg_openwsn  OpenWSN-specific implementation of the sock API
 * @ingroup  pkg_openwsn
 * @brief
 * @{
 *
 * @file
 * @brief
 *
 * @author  Michael Frey <michael.frey@msasafety.com>
 */
#ifndef SOCK_TYPES_H
#define SOCK_TYPES_H

#include <stdatomic.h>

#include "mbox.h"
#include "mutex.h"
#include "net/ipv6/addr.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SOCK_MBOX_SIZE
#define SOCK_MBOX_SIZE  (2)
#endif

#ifndef SOCK_HAS_IPV6
#error "OpenWSN only runs with IPv6 support"
#endif

/**
 * @brief @ref net_sock_udp definition for OpenWSN
 */
struct sock_udp {
    mutex_t mutex;                      /**< mutex for the connection */
    mbox_t mbox;                        /**< mbox for receiving */
    msg_t mbox_queue[SOCK_MBOX_SIZE];   /**< queue for mbox */
    atomic_int receivers;               /**< current number of recv calls */

    ipv6_addr_t source;                 /**< (local) source address */
    uint16_t port;                      /**< (local) source port */

    struct {
        const ipv6_addr_t *src;         /**< source address */
        size_t datalen;                 /**< length of received packet data */
        uint16_t src_port;              /**< source port */
        const void *data;               /**< data of received packet */
    } recv_info;                        /**< info on received packet */
};

#ifdef __cplusplus
}
#endif

#endif /* SOCK_TYPES_H */
/** @} */
