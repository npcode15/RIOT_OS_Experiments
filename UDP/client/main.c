#include <stdio.h>
#include <time.h>
#include <sys/time.h>
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
#include "xtimer.h"
#include "timex.h"

xtimer_ticks32_t timeStampI;
xtimer_ticks32_t timeStampII;
static int send_data=1;
char stack[THREAD_STACKSIZE_MAIN];
//char *ip_address = ""; //98
char *ip_address = "fe80::1711:6b10:65f6:5d02"; //101
//char *ip_address = "fe80::9414:7cff:fe82:39d5"; //tap0

void *rcv_pkt(void *arg);
void get_interface_and_address(void);
double time_diff(struct timeval x , struct timeval y);
static void send(char *addr_str, char *port_str, char *data, unsigned int num, unsigned int delay);

int main(void)
{
    static int cnt = 0;
//int a;
    thread_create(stack, sizeof(stack),
                    THREAD_PRIORITY_MAIN - 1,
                    THREAD_CREATE_STACKTEST,
                    rcv_pkt, NULL, "thread");

    while (cnt<1000) {
printf(".");
        if (send_data == 1){
            send_data = 0;
	    printf("cnt is: %i\n", cnt);
	    cnt = cnt + 1;

            timeStampI = xtimer_now();
	    printf("Sending UDP Packet to server #%d at %" PRIu32 "\n", cnt, xtimer_usec_from_ticks(timeStampI));
	    //printf("Timestamp I: %" PRIu32 "\n", xtimer_usec_from_ticks(timeStampI));
	    send(ip_address, "8888", "Hello Node M3-101", 1, 0);
//a=cnt-1;
	   get_interface_and_address();
           // printf("Loop %i completed", a);
	}
    }

    printf("Client Exiting");
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
  printf("Address method exiting!!");
}

void *rcv_pkt(void *arg) {
 (void) arg;

 int num = 8;
 msg_t msg_queue[num];
 msg_init_queue(msg_queue, num);

 gnrc_netreg_entry_t server = GNRC_NETREG_ENTRY_INIT_PID(8888, sched_active_pid);
 gnrc_netreg_register(GNRC_NETTYPE_UDP, &server);

 msg_t msg;
 while (1) {
    gnrc_pktsnip_t *pkt;
    msg_receive(&msg);
    pkt = (gnrc_pktsnip_t *)msg.content.ptr;

    timeStampII = xtimer_now();
    printf("Received UDP Packet from server at: %" PRIu32 "\n", xtimer_usec_from_ticks(timeStampII));
    //printf("Timestamp II: %" PRIu32 "\n", xtimer_usec_from_ticks(timeStampII));
    od_hex_dump(pkt->data, pkt->size, OD_WIDTH_DEFAULT);

    unsigned int rtt = xtimer_usec_from_ticks(timeStampII) - xtimer_usec_from_ticks(timeStampI);
    printf("Round_Trip_Time (RTT) is: %i\n", rtt);
    // printf("Round_Trip_Time (RTT) is: %" PRIu32 "\n", rtt);

    gnrc_pktbuf_release(pkt);
    send_data = 1;
    printf("send_data is: %i", send_data);
 }

 return NULL;
}
