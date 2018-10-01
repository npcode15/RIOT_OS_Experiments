#include <stdio.h>
#include <inttypes.h>
#include "net/gnrc.h"
#include "net/gnrc/ipv6.h"
#include "net/gnrc/netif.h"
#include "net/gnrc/netif/hdr.h"
#include "net/gnrc/udp.h"
#include "net/gnrc/pktdump.h"
#include "timex.h"
#include "utlist.h"
#include "xtimer.h"

static inline bool _is_iface(kernel_pid_t iface)
{
    return (gnrc_netif_get_by_pid(iface) != NULL);
}

static void send(char *addr_str, char *data)
{
    kernel_pid_t iface;
    uint8_t addr[GNRC_NETIF_L2ADDR_MAXLEN];
    size_t addr_len;
    gnrc_pktsnip_t *pkt, *hdr;
    gnrc_netif_hdr_t *nethdr;
    uint8_t flags = 0x00;

    /* parse interface */
    // iface = atoi(argv[1]);
    /* get interface, if available */
    iface = 3;
    printf("iface is: %i\n", iface);
    if (!_is_iface(iface)) {
        puts("error: invalid interface given");
        //return;
    }

    /* parse address */
    printf("Address is %s\n", addr_str);
    addr_len = gnrc_netif_addr_from_str(addr_str, addr);

    printf("Address Len is %i\n", addr_len);
    if (addr_len == 0) {
        if (strcmp(addr_str, "bcast") == 0) {
            flags |= GNRC_NETIF_HDR_FLAGS_BROADCAST;
            printf("Valid IP");
        }
        else {
            puts("error: invalid address given");
            return;
        }
    }

    /* put packet together */
    pkt = gnrc_pktbuf_add(NULL, data, strlen(data), GNRC_NETTYPE_UNDEF);
    if (pkt == NULL) {
        puts("error: packet buffer full");
        return;
    }
    hdr = gnrc_netif_hdr_build(NULL, 0, addr, addr_len);
    if (hdr == NULL) {
        puts("error: packet buffer full");
        gnrc_pktbuf_release(pkt);
        return;
    }
    LL_PREPEND(pkt, hdr);
    nethdr = (gnrc_netif_hdr_t *)hdr->data;
    nethdr->flags = flags;
    /* and send it */
    
    if (gnrc_netapi_send(iface, pkt) < 1) {
        puts("error: unable to send");
        gnrc_pktbuf_release(pkt);
        return;
    }
    else{
        printf("Success: Packet sent to %s\n", addr_str);
    }

    return;
}

void dummy_calls(void){
    send("", "");
}
