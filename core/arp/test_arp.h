/**@file test_arp.h
 *@brief this files contains all prototypes for the functions to test an ARP module
 *@author Syed Amaar Ahmad
 *@date  September 27, 2010
 */
#include "arp.h"
#include "finstypes.h"
#include <inttypes.h>

void gen_neighbor_list(char* fileName);

uint32_t read_IP_addrs();

void mimic_net_request(uint32_t IP_sender_addrs, uint64_t MAC_sender_addrs, struct ARP_message *request_ARP);

void mimic_net_reply(struct ARP_message *request_ARP_ptr, struct ARP_message *reply_ARP_ptr);

struct node* read_neighbor_list(char* fileName);

void init_recordsARP(char *fileName);

void fins_from_net(struct finsFrame *fins_frame, int task);

void fins_from_stub(struct finsFrame *fins_frame);

void arp_test_harness();



