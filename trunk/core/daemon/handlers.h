/*
 * handlers.h
 *
 *  Created on: Mar 6, 2011
 *      Author: Abdallah Abdallah
 */

#ifndef HANDLERS_H_
#define HANDLERS_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <poll.h>

#include <linux/netlink.h>
#include <linux/if_ether.h>
#include <pthread.h>
/** additional header for queues */
#include <queueModule.h>
/**additional headers for testing */
#include <finsdebug.h>
/** Additional header for meta-data manipulation */
#include <metadata.h>
//#include "arp.c"

/* to handle forking + pipes operations */
#include <unistd.h>
#include <sys/types.h>

#include "udpHandling.h"
#include "tcpHandling.h"
#include "icmpHandling.h"

/** FINS Sockets database related defined constants */
#define MAX_SOCKETS 100
#define MaxChildrenNumSharingSocket 100
#define MAX_parallel_threads 10
#define MAX_Queue_size 100000
#define MAX_parallel_processes 10
#define ACK 	200
#define NACK 	6666
#define MIN_port 32768
#define MAX_port 61000
#define DEFAULT_BACKLOG 5

/** Socket related calls and their codes */
#define socket_call 1
#define socketpair_call 2
#define bind_call 3
//#define getsockname_call 4
#define getname_call 4
#define connect_call 5
//#define getpeername_call 6
#define send_call 7
#define recv_call 8
#define sendto_call 9
#define recvfrom_call 10
#define sendmsg_call 11
#define recvmsg_call 12
#define getsockopt_call 13
#define setsockopt_call 14
#define listen_call 15
#define accept_call 16
#define accept4_call 17
#define shutdown_call 18
#define close_call 19
#define release_call 20
#define ioctl_call 21
#define daemonconnect_call 22
#define poll_call 23
#define mmap_call 24
#define sendpage_call 25

#define MAX_CALL_TYPES 26

//fins netlink stuff
#define NETLINK_FINS	20		// Pick an appropriate protocol or define a new one in include/linux/netlink.h
struct sockaddr_nl local_sockaddress; // sockaddr_nl for this process (source)
struct sockaddr_nl kernel_sockaddress; // sockaddr_nl for the kernel (destination)
int nl_sockfd; //temp for now
sem_t nl_sem;

enum sockOptions {

	FSO_DEBUG = 1,
	FSO_REUSEADDR,
	FSO_TYPE,
	FSO_ERROR,
	FSO_DONTROUTE,
	FSO_BROADCAST,
	FSO_SNDBUF,
	FSO_RCVBUF,
	FSO_KEEPALIVE,
	FSO_OOBINLINE,
	FSO_NO_CHECK,
	FSO_PRIORITY,
	FSO_LINGER,
	FSO_BSDCOMPAT, /** 14 */
	FSO_REUSEPORT = 15, /** FSO_REUSEPORT = 15 */
	FSO_PASSCRED = 16,
	FSO_PEERCRED,
	FSO_RCVLOWAT,
	FSO_SNDLOWAT,
	FSO_RCVTIMEO,
	FSO_SNDTIMEO, /** SO_SNDTIMEO	21 */

	FSO_BINDTODEVICE = 25,
	FSO_TIMESTAMP = 29,
	FSO_ACCEPTCONN = 30,
	FSO_PEERSEC = 31,
	FSO_SNDBUFFORCE = 32,
	FSO_RCVBUFFORCE = 33,

};

struct socket_Options {

	int FSO_DEBUG;
	int FSO_REUSEADDR;
	int FSO_TYPE;
	int FSO_PROTOCOL;
	int FSO_DOMAIN;
	int FSO_ERROR;
	int FSO_DONTROUTE;
	int FSO_BROADCAST;
	int FSO_SNDBUF;
	int FSO_SNDBUFFORCE;
	int FSO_RCVBUF;
	int FSO_RCVBUFFORCE;
	int FSO_KEEPALIVE;
	int FSO_OOBINLINE;
	int FSO_NO_CHECK;
	int FSO_PRIORITY;
	int FSO_LINGER;
	int FSO_BSDCOMPAT;
	int FSO_TIMESTAMP;
	int FSO_TIMESTAMPNS;
	int FSO_TIMESTAMPING;
	int FSO_RCVTIMEO;
	int FSO_SNDTIMEO;
	int FSO_RCVLOWAT;
	int FSO_SNDLOWAT;
	int FSO_PASSCRED;
	int FSO_PEERCRED;
	char FSO_PEERNAME[128];
	int FSO_ACCEPTCONN;
	int FSO_PASSSEC;
	int FSO_PEERSEC;
	int FSO_MARK;
	int FSO_RXQ_OVFL;
	int FSO_ATTACH_FILTER;
	int FSO_DETACH_FILTER;
};

struct tcp_Parameters {

	int SHUT_RD;
	int SHUT_WR;

};

//TODO merge with ipv4 stuff & create centralized IP/MAC/Device handling
uint32_t my_host_ip_addr; // = IP4_ADR_P2H(192,168,1,20);
uint32_t loopback_ip_addr; // = IP4_ADR_P2H(127,0,0,1);
uint32_t any_ip_addr; // = IP4_ADR_P2H(0,0,0,0);

struct nl_wedge_to_daemon {
	unsigned long long uniqueSockID; //TODO when ironed out remove uID or index, prob uID
	int index;

	u_int call_type;
	int call_threads;

	u_int call_id; //TODO when ironed out remove id or index
	int call_index;
};

struct nl_daemon_to_wedge {
	u_int call_type;
	u_int call_id; //TODO when ironed out remove id or index
	int call_index;

	unsigned long long uniqueSockID; //TODO when ironed out remove uID or index
	int index;

	u_int ret;
	u_int msg;
};

struct fins_daemon_socket {
	/** variables tells a connect call has been called over this socket or not in order to
	 * check the address of the senders of the received datagrams against the address which this
	 * socket is connected to it before approving or dropping any datagram
	 */
	//int connection_status; //0=created, not connected to anything, 1=connecting/accepting, 2=established
	socket_state state;
	unsigned long long uniqueSockID;
	pid_t childrenList[MaxChildrenNumSharingSocket];
	int type;
	int protocol;

	sem_t sem; //TODO implement? would need for multithreading
	int listening;
	int backlog;

	/** check the opt_name to find which bit to access in the options variable then use
	 * the following code to handle the bits individually
	 * setting a bit   number |= 1 << x;  That will set bit x.
	 * Clearing a bit number &= ~(1 << x); That will clear bit x.
	 * The XOR operator (^) can be used to toggle a bit. number ^= 1 << x; That will toggle bit x.
	 * Checking a bit      value = number & (1 << x);
	 */
	//uint32_t socketoptions;
	struct socket_Options sockopts;
	int blockingFlag;
	struct tcp_Parameters tcpParameters;

	/** All the above already initialized using the insert function
	 * the remaining below is handled using the update function*/
	uint32_t host_ip; //host format
	uint16_t host_port; //host format
	uint32_t dst_ip; //host format
	uint16_t dst_port; //host format
	char name[50];
	int data_pipe[2];

	sem_t Qs; /** The data Queue Semaphore Pointer*/
	finsQueue controlQueue;
	sem_t control_sem;

	finsQueue dataQueue;
	sem_t data_sem;
	int buf_data;

	int recv_ind;
	int threads;
	int replies;
};

//ADDED mrd015 !!!!! (this crap really needs to be gathered into one header.)
#ifdef BUILD_FOR_ANDROID
#define FINS_TMP_ROOT "/data/data/fins"
#else
#define FINS_TMP_ROOT "/tmp/fins"
#endif

#define MAIN_SOCKET_CHANNEL FINS_TMP_ROOT "/mainsocket_channel"
#define CLIENT_CHANNEL_TX FINS_TMP_ROOT "/uniqueSockID_%llu_TX"
#define CLIENT_CHANNEL_RX FINS_TMP_ROOT "/uniqueSockID_%llu_RX"
#define RTM_PIPE_IN FINS_TMP_ROOT "/rtm_in"
#define RTM_PIPE_OUT FINS_TMP_ROOT "/rtm_out"

int init_fins_nl();
int send_wedge(int sockfd, u_char *buf, size_t len, int flags);

int nack_send_new(unsigned long long uniqueSockID, int index, u_int call_id, int call_index, u_int call_type, u_int ret_msg);
int nack_send(unsigned long long uniqueSockID, u_int socketCallType, u_int ret_msg);
int ack_send_new(unsigned long long uniqueSockID, int index, u_int call_id, int call_index, u_int call_type, u_int ret_msg);
int ack_send(unsigned long long uniqueSockID, u_int socketCallType, u_int ret_msg);

void init_daemonSockets();
int randoming(int min, int max);
int check_daemonSocket(unsigned long long uniqueSockID);
int match_daemonSocket(uint16_t dstport, uint32_t dstip, int protocol);
int match_daemon_connection(uint32_t host_ip, uint16_t host_port, uint32_t rem_ip, uint16_t rem_port, int protocol);
int find_daemonSocket(unsigned long long uniqueSockID);
int insert_daemonSocket_new(unsigned long long uniqueSockID, int index, int type, int protocol);
int insert_daemonSocket(unsigned long long uniqueSockID, int type, int protocol);
int remove_daemonSocket_new(unsigned long long uniqueSockID, int index);
int remove_daemonSocket(unsigned long long uniqueSockID);

int check_daemon_ports(uint16_t hostport, uint32_t hostip);

struct finsFrame *get_fdf_old(int index, unsigned long long uniqueSockID, int non_block_flag); //deprecated
struct finsFrame *get_fcf_old(int index, unsigned long long uniqueSockID, int non_block_flag); //deprecated //blocking doesn't matter

int get_fdf(int index, unsigned long long uniqueSockID, struct finsFrame **ff, int non_blocking_flag);
int get_fcf(int index, unsigned long long uniqueSockID, struct finsFrame **ff, int non_blocking_flag); //blocking doesn't matter

/** calls handling functions */
void socket_call_handler(unsigned long long uniqueSockID, int index, int call_threads, u_int call_id, int call_index, u_char *buf, ssize_t len);
void bind_call_handler(unsigned long long uniqueSockID, int index, int call_threads, u_int call_id, int call_index, u_char *buf, ssize_t len);
void listen_call_handler(unsigned long long uniqueSockID, int index, int call_threads, u_int call_id, int call_index, u_char *buf, ssize_t len);
void connect_call_handler(unsigned long long uniqueSockID, int index, int call_threads, u_int call_id, int call_index, u_char *buf, ssize_t len);

void accept_call_handler(unsigned long long uniqueSockID, int threads, u_char *buf, ssize_t len);
void getname_call_handler(unsigned long long uniqueSockID, int threads, u_char *buf, ssize_t len);
void sendmsg_call_handler(unsigned long long uniqueSockID, int threads, u_char *buf, ssize_t len);
void recvmsg_call_handler(unsigned long long uniqueSockID, int threads, u_char *buf, ssize_t len);

void ioctl_call_handler(unsigned long long uniqueSockID, int index, int call_threads, u_int call_id, int call_index, u_char *buf, ssize_t len);

void getsockopt_call_handler(unsigned long long uniqueSockID, int threads, u_char *buf, ssize_t len);
void setsockopt_call_handler(unsigned long long uniqueSockID, int threads, u_char *buf, ssize_t len);

void release_call_handler(unsigned long long uniqueSockID, int index, int call_threads, u_int call_id, int call_index, u_char *buf, ssize_t len);

void poll_call_handler(unsigned long long uniqueSockID, int threads, u_char *buf, ssize_t len);
void mmap_call_handler(unsigned long long uniqueSockID, int threads, u_char *buf, ssize_t len);

void socketpair_call_handler();
void accept4_call_handler(unsigned long long uniqueSockID, int threads, u_char *buf, ssize_t len);
void shutdown_call_handler(unsigned long long uniqueSockID, int threads, u_char *buf, ssize_t len);

void close_call_handler(unsigned long long uniqueSockID, int threads, u_char *buf, ssize_t len);
//void getsockname_call_handler(unsigned long long uniqueSockID, int threads, u_char *buf, ssize_t len);
//void getpeername_call_handler(unsigned long long uniqueSockID, int threads, u_char *buf, ssize_t len);
//void send_call_handler(unsigned long long uniqueSockID, int threads, u_char *buf, ssize_t len);
//void recv_call_handler(unsigned long long uniqueSockID, int threads, u_char *buf, ssize_t len);
//void sendto_call_handler(unsigned long long uniqueSockID, int threads, u_char *buf, ssize_t len);
//void recvfrom_call_handler(unsigned long long uniqueSockID, int threads, u_char *buf, ssize_t len);

//######################### //TODO remove
struct recvfrom_data {
	int id;
	unsigned long long uniqueSockID;
	int socketCallType;
	int datalen;
	int flags;
	int symbol;
};
void recvthread_exit(struct recvfrom_data *thread_data);
//#########################

#endif /* HANDLERS_H_ */
