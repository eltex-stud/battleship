/* vim: set noexpandtab tabstop=4 shiftwidth=4 smartindent: */

#include <pthread.h>
#include "client.h"
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
	pthread_mutex_t *mutex;
};

