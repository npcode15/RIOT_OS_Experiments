#include <stdio.h>
#include <time.h>
#include "msg.h"
#include "net/ipv6/addr.h"
#include "net/gnrc.h"
#include "net/gnrc/netif.h"
#include "net/gnrc/netreg.h"
#include "net/gnrc/pktbuf.h"
#include "thread.h"
#include "sched.h"
#include "od.h"
#include "udp.c"

char *ip_address = "fe80::1711:6b10:65fc:5c22"; //100
//char *ip_address = "fe80::1711:6b10:65fc:5c22"; //100
//char *ip_address = "fe80::c1e:bcff:fedb:ef5f"; //Tap1

char stack[THREAD_STACKSIZE_MAIN];
void *rcv_pkt(void *arg);
void get_interface_and_address(void);
static void send(char *addr_str, char *port_str, char *data, unsigned int num, unsigned int delay);

int main(void)
{
    thread_create(stack, sizeof(stack),
                    THREAD_PRIORITY_MAIN - 1,
                    THREAD_CREATE_STACKTEST,
                    rcv_pkt, NULL, "thread");

    get_interface_and_address();

    return 0;
}

void get_interface_and_address(void){
   /* get interfaces and print their addresses */
    gnrc_netif_t *netif = NULL;
    while ((netif = gnrc_netif_iter(netif))) {
        ipv6_addr_t ipv6_addrs[GNRC_NETIF_IPV6_ADDRS_NUMOF];
        int res = gnrc_netapi_get(netif->pid, NETOPT_IPV6_ADDR, 0, ipv6_addrs,
                                  sizeof(ipv6_addrs));

        if (res < 0) {
            continue;
        }
        for (unsigned i = 0; i < (unsigned)(res / sizeof(ipv6_addr_t)); i++) {
            char ipv6_addr[IPV6_ADDR_MAX_STR_LEN];

            ipv6_addr_to_str(ipv6_addr, &ipv6_addrs[i], IPV6_ADDR_MAX_STR_LEN);
            printf("My address is %s\n", ipv6_addr);
        }
    }
}

void *rcv_pkt(void *arg) {
 (void) arg;

 int num = 4;
 msg_t msg_queue[num];
 msg_init_queue(msg_queue, num);

 gnrc_netreg_entry_t server = GNRC_NETREG_ENTRY_INIT_PID(8888, sched_active_pid);
 gnrc_netreg_register(GNRC_NETTYPE_UDP, &server);

 msg_t msg;
 while (1) {
    gnrc_pktsnip_t *pkt;
    msg_receive(&msg);
    pkt = (gnrc_pktsnip_t *)msg.content.ptr;

    //printf("NETTYPE_UNDEF (%i)\n", pkt->type);
    od_hex_dump(pkt->data, pkt->size, OD_WIDTH_DEFAULT);
    //udp_hdr_print(pkt->data);
    //od_hex_dump(pkt->size, OD_WIDTH_DEFAULT);

    //xtimer_sleep(2);
    printf("Sending Reply\n");
    send(ip_address, "8888", "Hello Node M3-100", 1, 0);

    gnrc_pktbuf_release(pkt);
 }

 return NULL;
}
