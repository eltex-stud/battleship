/* vim: set noexpandtab tabstop=4 shiftwidth=4 smartindent: */

#ifndef CL_NET
#define CL_NET

#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <error.h>
#include "client.h"
#include "cl_main.h"
#include "../include/net_types.h"

struct net_queue {
	struct net_queue *net_next;
	char type_msg;
	void *data;
	int data_len;
};

struct net {
	struct net_queue *net_queue_head;
	struct main_queue *m_queue;
	int socket;
	pthread_t pthreadfd;
	pthread_mutex_t mutex;
};

struct net *cl_net_start(char address, int port, struct main_queue *m_queue);
void *net_work(void *arg);
void cl_net_processing_event(struct net *configure);
void cl_net_send_placement(struct net *configure, placement *net_placement);
void cl_net_send_short(struct net *configure, char shot_x, char shot_y);
void cl_net_send_nick(struct net *configure, char *net_nick, int nick_len);
void cl_net_send_start(struct net *configure);
void cl_net_send_error(struct net *configure, int net_error);
void cl_net_add_queue(struct net *configure, struct net_queue *element);
void cl_net_del_queue(struct net_queue *net_queue_head);
void cl_net_recv(struct net *configure);

#endif
