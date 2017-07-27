/*
 * Copyright (C) 2017 MSA Safety
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @{
 *
 * @file
 * @author  Michael Frey <michael.frey@msasafety.com>
 */

#include <errno.h>
#include <assert.h>
#include <stdint.h> 

#include "opendefs.h"
#include "02a-MAClow/IEEE802154E.h"
#include "cross-layers/openqueue.h"
#include "04-TRAN/openudp.h"
#include "cross-layers/packetfunctions.h"
#include "cross-layers/idmanager.h"

#include "mutex.h"
#include "net/ipv6/addr.h"
#include "sock_types.h"
#include "byteorder.h"
#include "msg.h"
#include "mutex.h"
#include "net/af.h"
#include "net/sock/udp.h"
#include "net/ipv6/hdr.h"
#include "sched.h"
#include "xtimer.h"

#define ENABLE_DEBUG(0)
#include "debug.h"

#define _MSG_TYPE_CLOSE     (0x5123)
#define _MSG_TYPE_TIMEOUT   (0x5124)
#define _MSG_TYPE_RCV       (0x5125)

typedef struct _openwsn_data {
    /** */
    size_t length;
    /** store the result of the recv operation */
    int result;
    /** */
    uint8_t *payload;
} _openwsn_data_t;

static mbox_t *queue;

void _receive_callback(OpenQueueEntry_t* packet);

void _receive_callback(OpenQueueEntry_t* packet)
{
    _openwsn_data_t data;
    /** set the payload length */
    data->length = packet->l4_length;
    /** set the payload */
    data->payload = packet->l4_payload;

    /** wrap it up in a message */
    msg_t msg;
    msg.type = _MSG_TYPE_RCV;
    msg.ptr = &data;

    mbox_try_put(queue, &msg);
}

static void _timeout_callback(void *arg)
{
    msg_t msg = { .type = _MSG_TYPE_TIMEOUT };
    mbox_t *mbox = arg;
    mbox_try_put(mbox, &msg);
}

int sock_udp_create(sock_udp_t *sock, const sock_udp_ep_t *local,
                    const sock_udp_ep_t *remote, uint16_t flags)
{
    if (((local != NULL) && (local->family != AF_INET6)) ||
        ((remote != NULL) && (remote->family != AF_INET6))) {
        return -EAFNOSUPPORT;
    }

    mutex_init(&(sock->mutex));
    /** initialize mbox */
    mutex_lock(&(sock->mutex));
    mbox_init(&sock->mbox, sock->mbox_queue, SOCK_MBOX_SIZE);

    /** TODO: does it work? */
    queue = &(sock->mbox);

    atomic_flag_clear(&sock->receivers);
    mutex_unlock(&(sock->mutex));

    udp_resource_desc_t description;
    /** set receive callback */
    description.callbackReceive = _receive_callback;
    /** 
     * if the callback is set to NULL messages are discarded 
     * after they have been sent. if there should be a more sophisticated 
     * message handling (retries, etc.), this probably needs to be handled in 
     * description.callbackSendDone
     */
    description.callbackSendDone = NULL;
    /** set port */
    description.port = sock->port;

    /** save udp descriptor */
    openudp_register(&description);

    return 0;
}

void sock_udp_close(sock_udp_t *sock)
{
    udp_resource_desc_t description;
    /** reset values of UDP resource description */
    description.port             = 0;    /** UDP port 0 is valid, but well ... */
    description.callbackReceive  = NULL;
    description.callbackSendDone = NULL;
    /** reset udp descriptor */
    openudp_register(&description);
}


int sock_udp_recv(sock_udp_t *sock, void *data, size_t max_len,
                  uint32_t timeout, sock_udp_ep_t *remote)
{
    xtimer_t timeout_timer;
    int blocking = BLOCKING;
    int result = -EIO;
    msg_t msg;

    assert((sock != NULL) && (data != NULL) && (max_len > 0));

    if (timeout == 0) {
        blocking = NON_BLOCKING;
    } else if (timeout != SOCK_NO_TIMEOUT) {
        timeout_timer.callback = _timeout_callback;
        timeout_timer.arg = &sock->mbox;
        xtimer_set(&timeout_timer, timeout);
    }

    atomic_fetch_add(&sock->receivers, 1);
    if (_mbox_get(&sock->mbox, &msg, blocking) == 0) {
        return -EAGAIN;
    }

    switch (msg.type) {
        case _MSG_TYPE_CLOSE:
            result = -EADDRNOTAVAIL;
            break;
        case _MSG_TYPE_TIMEOUT:
            result = -ETIMEDOUT;
            break;
        case _MSG_TYPE_RCV:
            /** try to acquire mutex */
            mutex_lock(&(sock->mutex));
            _openwsn_data_t *packet = (_openwsn_data_t*)msg.ptr;

            /** size of received data <= expected */
            if (packet->length <= max_len) {
                memcpy(data, packet->payload, packet->length);
                result = packet->length;
                /** copy over 'len' bytes, since the received payload is too large */
            } else {
                /** TODO: check */
                memcpy(data, packet->payload, max_len);
                result = -ENOBUFS;
            }

            if (remote != NULL) {
                remote->family = AF_INET6;
                remote->netif = SOCK_ADDR_ANY_NETIF;
                memcpy(&remote->addr, &sock->recv_info.src, sizeof(ipv6_addr_t));
                remote->port = sock->recv_info.src_port;
            }
    }

    atomic_fetch_sub(&sock->receivers, 1);
    return result;
}

int sock_udp_get_local(sock_udp_t *sock, sock_udp_ep_t *ep)
{
    assert((sock != NULL) && (ep != NULL));

    ipv6_addr_t address;

    open_addr_t *prefix = idmanager_getMyID(ADDR_PREFIX);
    memcpy(&(address.u8[0]), prefix, 8);

    open_addr_t *local = idmanager_getMyID(ADDR_64B);
    memcpy(&(address.u8[8]), local, 8);

    mutex_lock(&sock->mutex);
    /** set local endpoint address */
    memcpy(&(sock->source), &address, sizeof(ipv6_addr_t));
    mutex_unlock(&sock->mutex);

    return 0;
}

int sock_udp_get_remote(sock_udp_t *sock, sock_udp_ep_t *ep)
{
    assert((sock != NULL) && (ep != NULL));

    mutex_lock(&sock->mutex);
    // TODO
    //memcpy(&ep->addr, &sock->sock.udp_conn->ripaddr, sizeof(ipv6_addr_t));
    //ep->port = ntohs(sock->sock.udp_conn->rport);
    mutex_unlock(&sock->mutex);

    return 0;
}

int sock_udp_send(sock_udp_t *sock, const void *data, size_t len,
                const sock_udp_ep_t *remote)
{
    assert((sock != NULL) || (remote != NULL));
    /* (len != 0) => (data != NULL) */
    assert((len == 0) || (data != NULL));   

    OpenQueueEntry_t *packet;
    /** allocate packet */
    packet = openqueue_getFreePacketBuffer(COMPONENT_RIOT);

    if (packet != NULL) {
        uint8_t absoluteSlotNumber[5];

        packet->owner                         = COMPONENT_RIOT;
        packet->creator                       = COMPONENT_RIOT;

        /** set address types */
        packet->l3_sourceAdd.type             = ADDR_128B;
        packet->l3_destinationAdd.type        = ADDR_128B;
        /** set source address */
        memcpy(&packet->l3_sourceAdd.addr_128b[0], &(sock->source), sizeof(ipv6_addr_t));
        /** set destination address */
        memcpy(&packet->l3_destinationAdd.addr_128b[0], remote->addr.ipv6, sizeof(ipv6_addr_t));

        /** reserve some space for the payload */
        packetfunctions_reserveHeaderSize(packet, len);
        /** copy over payload */
        memcpy(packet->l4_payload, data, len);
        packet->l4_protocol                   = IANA_UDP;
        packet->l4_destination_port           = remote->port;
        packet->l4_sourcePortORicmpv6Type     = sock->port;

        /***/
        //packetfunctions_reserveHeaderSize(packet, sizeof(uint16_t));

        /** reserve some space for the absolute slot size */
        packetfunctions_reserveHeaderSize(packet, sizeof(asn_t));
        ieee154e_getAsn(absoluteSlotNumber);
        /** set slot number */
        packet->payload[0] = absoluteSlotNumber[0];
        packet->payload[1] = absoluteSlotNumber[1];
        packet->payload[2] = absoluteSlotNumber[2];
        packet->payload[3] = absoluteSlotNumber[3];
        packet->payload[4] = absoluteSlotNumber[4];

        /** send udp packet */
        owerror_t result = openudp_send(packet);

        if (result == E_FAIL) {
            openqueue_freePacketBuffer(packet);
#if ENABLE_DEBUG
            printf("Failed to send UDP packet: %d\n", (int)result);
#endif 
            return -2;
        } 

        return 0;
    } 

#if ENABLE_DEBUG
    printf("could not send udp packet, memory allocation failed\n");
#endif 
    return -1;
}
