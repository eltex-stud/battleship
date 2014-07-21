/* vim: set noexpandtab tabstop=4 shiftwidth=4 smartindent: */

#include <pthread.h>

struct net_queue {
	struct net_queue *net_next;
	struct net_queue *net_head;
	char net_event;
};
