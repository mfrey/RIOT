/*
 * Copyright (C) 2017 Univiversity of Applied Sciences
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     tests
 * @{
 *
 * @file
 * @brief       OpenWSN test application
 *
 * @author      Peter Kietzmann <peter.kietzmann@haw-hamburg.de>
 *
 * @}
 */

#include <stdio.h>

#include "opendefs.h"
#include "openstack.h"
#include "board_ow.h"
#include "02a-MAClow/IEEE802154E.h"
#include "04-TRAN/openudp.h"
#include "cross-layers/openqueue.h"
#include "cross-layers/packetfunctions.h"
#include "cross-layers/idmanager.h"

typedef struct {
   uint16_t             counter;  ///< incrementing counter which is written into the packet
   uint16_t              period;  ///< uinject packet sending period>
   udp_resource_desc_t     desc;  ///< resource descriptor for this module, used to register at UDP stack
} uinject_vars_t;

uinject_vars_t uinject_vars;

void sniffer_setListeningChannel(uint8_t channel){return;}

static const uint8_t uinject_payload[]    = "uinject";
static const uint8_t uinject_dst_addr[]   = {
   0xbb, 0xbb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
}; 

extern ieee154e_vars_t ieee154e_vars;
extern idmanager_vars_t idmanager_vars;

int main(void)
{
    puts("OpenWSN UDP test!");

    printf("You are running RIOT on a(n) %s board.\n", RIOT_BOARD);
    printf("This board features a(n) %s MCU.\n", RIOT_MCU);

    board_init_ow();

    openstack_init();
    OpenQueueEntry_t*    pkt;
    uint8_t              asnArray[5];
/*
    // don't run if not synch
    if (ieee154e_isSynch() == FALSE) return;

    // don't run on dagroot
    if (idmanager_getIsDAGroot()) {
      opentimers_destroy(uinject_vars.timerId);
      return;
    }
*/
    // if you get here, send a packet
    // Fake that you are in sync for debuging
    ieee154e_vars.isSync = true;
    //idmanager_vars.isDAGroot = true; // needed
    if (idmanager_vars.isDAGroot == true){
        puts("Node is DAG root");
    }
    else{
        puts("Node is DAG leaf");
    }
    // get a free packet buffer
    pkt = openqueue_getFreePacketBuffer(COMPONENT_UINJECT);
    if(pkt == NULL){
        puts("ERROR could not create packet buffer");
        return -1;
    }
    puts("got free packet buffer");
/*    if (pkt==NULL) {
      openserial_printError(
         COMPONENT_UINJECT,
         ERR_NO_FREE_PACKET_BUFFER,
         (errorparameter_t)0,
         (errorparameter_t)0
      );
      return -1;
    }*/

    pkt->owner                         = COMPONENT_UINJECT;
    puts("write to pkt->owner");
    pkt->creator                       = COMPONENT_UINJECT;
    pkt->l4_protocol                   = IANA_UDP;
    pkt->l4_destination_port           = WKP_UDP_INJECT;
    pkt->l4_sourcePortORicmpv6Type     = WKP_UDP_INJECT;
    pkt->l3_destinationAdd.type        = ADDR_128B;
    memcpy(&pkt->l3_destinationAdd.addr_128b[0],uinject_dst_addr,16);
    // add payload
    packetfunctions_reserveHeaderSize(pkt,sizeof(uinject_payload)-1);
    memcpy(&pkt->payload[0],uinject_payload,sizeof(uinject_payload)-1);
    packetfunctions_reserveHeaderSize(pkt,sizeof(uint16_t));
    pkt->payload[1] = (uint8_t)((uinject_vars.counter & 0xff00)>>8);
    pkt->payload[0] = (uint8_t)(uinject_vars.counter & 0x00ff);
    uinject_vars.counter++;
    packetfunctions_reserveHeaderSize(pkt,sizeof(asn_t));
    ieee154e_getAsn(asnArray);
    pkt->payload[0] = asnArray[0];
    pkt->payload[1] = asnArray[1];
    pkt->payload[2] = asnArray[2];
    pkt->payload[3] = asnArray[3];
    pkt->payload[4] = asnArray[4];

    owerror_t ret = openudp_send(pkt);
    if (ret==E_FAIL) {
      openqueue_freePacketBuffer(pkt);
      printf("Failed to send UDP packet: %i\n", (int)ret);
    }
    puts("OpenWSN UDP test end");
    return 0;
}
