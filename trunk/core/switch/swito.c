/**
 * @file swito.c
 *
 *  @date Mar 14, 2011
 *      @author Abdallah Abdallah
 */

#include "swito.h"
#include <finstypes.h>
#include <metadata.h>
#include <queueModule.h>
#include <arpa/inet.h>

int switch_running;
#define MAX_modules 16
extern finsQueue Daemon_to_Switch_Queue;
extern finsQueue Switch_to_Daemon_Queue;

extern finsQueue RTM_to_Switch_Queue;
extern finsQueue Switch_to_RTM_Queue;

extern finsQueue Switch_to_UDP_Queue;
extern finsQueue UDP_to_Switch_Queue;

extern finsQueue Switch_to_TCP_Queue;
extern finsQueue TCP_to_Switch_Queue;

extern finsQueue Switch_to_ARP_Queue;
extern finsQueue ARP_to_Switch_Queue;

extern finsQueue Switch_to_IPv4_Queue;
extern finsQueue IPv4_to_Switch_Queue;

extern finsQueue Switch_to_EtherStub_Queue;
extern finsQueue EtherStub_to_Switch_Queue;

extern finsQueue Switch_to_ICMP_Queue;
extern finsQueue ICMP_to_Switch_Queue;

extern sem_t ICMP_to_Switch_Qsem;
extern sem_t Switch_to_ICMP_Qsem;

extern sem_t RTM_to_Switch_Qsem;
extern sem_t Switch_to_RTM_Qsem;

extern sem_t Daemon_to_Switch_Qsem;
extern sem_t Switch_to_Daemon_Qsem;

extern sem_t Switch_to_UDP_Qsem;
extern sem_t UDP_to_Switch_Qsem;

extern sem_t Switch_to_TCP_Qsem;
extern sem_t TCP_to_Switch_Qsem;

extern sem_t Switch_to_IPv4_Qsem;
extern sem_t IPv4_to_Switch_Qsem;

extern sem_t Switch_to_ARP_Qsem;
extern sem_t ARP_to_Switch_Qsem;

extern sem_t Switch_to_EtherStub_Qsem;
extern sem_t EtherStub_to_Switch_Qsem;

extern finsQueue modules_IO_queues[MAX_modules];
extern sem_t *IO_queues_sem[MAX_modules];

void switch_init() {
	PRINT_DEBUG("Switch Started");
	switch_running = 1;

	int i;
	struct finsFrame *ff;

	int counter = 0;

	while (switch_running) {
		/** the receiving Queues are only the even numbers
		 * 0,2,4,6,8,10,12,14. This is why we increase the counter by 2
		 */
		for (i = 0; i < MAX_modules; i = i + 2) {

			sem_wait(IO_queues_sem[i]);
			ff = read_queue(modules_IO_queues[i]);
			sem_post(IO_queues_sem[i]);

			if (ff != NULL) {
				counter++;
				PRINT_DEBUG("Counter %d", counter);

				switch (ff->destinationID.id) {

				case ARPID:
					PRINT_DEBUG("ARP Queue +1, ff=%x", (int) ff);
					sem_wait(&Switch_to_ARP_Qsem);
					write_queue(ff, Switch_to_ARP_Queue);
					sem_post(&Switch_to_ARP_Qsem);
					break;
				case RTMID:
					PRINT_DEBUG("RTM Queue +1, ff=%x", (int) ff);
					sem_wait(&Switch_to_RTM_Qsem);
					write_queue(ff, Switch_to_RTM_Queue);
					sem_post(&Switch_to_RTM_Qsem);
					break;
				case DAEMONID:
					PRINT_DEBUG("Daemon Queue +1, ff=%x", (int) ff);
					sem_wait(&Switch_to_Daemon_Qsem);
					write_queue(ff, Switch_to_Daemon_Queue);
					sem_post(&Switch_to_Daemon_Qsem);
					break;
				case UDPID:
					PRINT_DEBUG("UDP Queue +1, ff=%x", (int) ff);
					sem_wait(&Switch_to_UDP_Qsem);
					write_queue(ff, Switch_to_UDP_Queue);
					sem_post(&Switch_to_UDP_Qsem);
					break;
				case TCPID:
					PRINT_DEBUG("TCP Queue +1, ff=%x", (int) ff);
					sem_wait(&Switch_to_TCP_Qsem);
					write_queue(ff, Switch_to_TCP_Queue);
					sem_post(&Switch_to_TCP_Qsem);
					break;
				case IPV4ID:
					PRINT_DEBUG("IP Queue +1, ff=%x", (int) ff);
					sem_wait(&Switch_to_IPv4_Qsem);
					write_queue(ff, Switch_to_IPv4_Queue);
					sem_post(&Switch_to_IPv4_Qsem);
					break;
				case ETHERSTUBID:
					PRINT_DEBUG("EtherStub Queue +1, ff=%x", (int) ff);
					sem_wait(&Switch_to_EtherStub_Qsem);
					write_queue(ff, Switch_to_EtherStub_Queue);
					sem_post(&Switch_to_EtherStub_Qsem);
					break;
				case ICMPID:
					PRINT_DEBUG("ICMP Queue +1, ff=%x", (int) ff);
					sem_wait(&Switch_to_ICMP_Qsem);
					write_queue(ff, Switch_to_ICMP_Queue);
					sem_post(&Switch_to_ICMP_Qsem);
					break;
				default:
					PRINT_DEBUG("Unknown Destination");
					freeFinsFrame(ff);
					break;
				} // end of Switch statement
			} // end of if (ff != NULL )
			else { //PRINT_DEBUG("No frame read from Queue # %d", i);

			}

		} //end of for For loop (Round Robin reading from Modules)

	} // end of while loop

	PRINT_DEBUG("Switch Terminating");
} // end of switch_init Function

void switch_term() {
	switch_running = 0;
}
