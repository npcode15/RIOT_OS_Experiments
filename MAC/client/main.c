#include <stdio.h>
#include "msg.h"
#include "net/ipv6/addr.h"
#include "net/gnrc.h"
#include "net/gnrc/netif.h"
#include "net/gnrc/netreg.h"
#include "net/gnrc/pktbuf.h"
#include "net/gnrc/pktdump.h"
#include "thread.h"
#include "sched.h"
#include "xtimer.h"
#include "od.h"
#include "mac.c"
//#include "timex.h"

xtimer_ticks32_t timeStampI;
xtimer_ticks32_t timeStampII;
static int send_data=1;
char stack[THREAD_STACKSIZE_MAIN];
// char *mac_address = "52:02:5f:e7:d0:57"; //tap0
char *mac_address = "15:11:6b:10:65:f6:5d:02"; //m3-101

void *rcv_pkt(void *arg);
static void send(char *addr_str, char *data);

int main(void)
{
    puts("RIOT Experiment - node-to-node radio communication");

    static int cnt = 0;

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
	    printf("Sending Radio Packet to server #%d at %" PRIu32 "\n", cnt, xtimer_usec_from_ticks(timeStampI));
	    send(mac_address, "Hello Node M3-100");
	}
    }

    printf("Client Exiting");
return 0;
}

void *rcv_pkt(void *arg) {

 (void) arg;

 int num = 4;
 msg_t msg_queue[num];
 msg_init_queue(msg_queue, num);

 /* initialize and register pktdump to print received packets */
 gnrc_netreg_entry_t server = GNRC_NETREG_ENTRY_INIT_PID(GNRC_NETREG_DEMUX_CTX_ALL,
                                sched_active_pid);
 gnrc_netreg_register(GNRC_NETTYPE_UNDEF, &server);

 msg_t msg;
 while (1) {
    msg_receive(&msg);
    gnrc_pktsnip_t *pkt;
    pkt = (gnrc_pktsnip_t *)msg.content.ptr;

    timeStampII = xtimer_now();
    printf("Received UDP Packet from server at: %" PRIu32 "\n", xtimer_usec_from_ticks(timeStampII));

    od_hex_dump(pkt->data, pkt->size, OD_WIDTH_DEFAULT);
    unsigned int rtt = xtimer_usec_from_ticks(timeStampII) - xtimer_usec_from_ticks(timeStampI);
    printf("Round_Trip_Time (RTT) is: %i\n", rtt);

    send_data = 1;
    gnrc_pktbuf_release(pkt);
 }

 return NULL;
}