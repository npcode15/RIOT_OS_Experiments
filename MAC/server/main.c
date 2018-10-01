#include <stdio.h>
#include <time.h>
#include "msg.h"
#include "net/ipv6/addr.h"
#include "net/gnrc.h"
#include "net/gnrc/netif.h"
#include "net/gnrc/netreg.h"
#include "net/gnrc/pktbuf.h"
#include "net/gnrc/pktdump.h"
#include "thread.h"
#include "sched.h"
#include "mac.c"
#include "od.h"
#include "shell.h"

// char *mac_address = "8a:2e:19:4e:ac:b0"; //tap1
char *mac_address = "15:11:6b:10:65:fc:5c:22"; //m-100

char stack[THREAD_STACKSIZE_MAIN];
void *rcv_pkt(void *arg);
void get_interface_and_address(void);
static void send(char *addr_str, char *data);

int main(void)
{
    puts("RIOT Experiment - node-to-node radio communication");

    thread_create(stack, sizeof(stack),
                    THREAD_PRIORITY_MAIN - 1,
                    THREAD_CREATE_STACKTEST,
                    rcv_pkt, NULL, "thread");

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

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

    od_hex_dump(pkt->data, pkt->size, OD_WIDTH_DEFAULT);

    printf("Sending Reply\n");
    send(mac_address, "Hello Node M3-100");

    gnrc_pktbuf_release(pkt);
 }

 return NULL;
}