/* vim: set noexpandtab tabstop=4 shiftwidth=4 smartindent: */
/*
 *The file cl_net.c/
 *
 */

#include "cl_net.h"
//#include "cl_main.h" // DELETE ME
#include <signal.h>
#include <pthread.h>
#include <strings.h>
#include <arpa/inet.h>


/*
 * NAME: cl_net_processing_event
 *
 * IN:	 struct net *configure - configure which was filled by net_start and
 * 		main
 *
 * OUT:	 nothing
 *
 * WORK: takes configure and element from the net queue, if configure->mutex
 *       is not locked, then cofigure->mutex locked, formed net message 
 *       and send to server,
 *       else waiting for unlock and do the same things
 */
int cl_net_processing_event(struct net *configure)
{
	struct net_queue *idx; /* iterator struct net_queue */
	char buf[SIZE_BUF];
	pthread_mutex_lock(&configure->mutex);
	if(configure->net_queue_head != NULL) {
		idx = configure->net_queue_head;
		if(idx->type_msg == END) {
			pthread_mutex_unlock(&configure->mutex);
			close(configure->socket);
			//pthread_kill(configure->pthreadfd, SIGKILL);
			free(configure);
			return -1;
		}
		memcpy(buf, (char*)&(idx->type_msg), 1);
		memcpy(buf + 1, idx->data, idx->data_len);
		send(configure->socket, buf, SIZE_BUF, 0);
		cl_net_del_queue(configure);
	}
	pthread_mutex_unlock(&configure->mutex);
	return 0;
}



/*
 * NAME: cl_net_start
 *
 * IN:	 char address - IP-address server
 * 		 int port - port server
 *
 * OUT:	 nothing
 *
 * WORK: takes configure and element from the net queue, if configure->mutex
 *       is not locked, then cofigure->mutex locked, formed net message 
 *       and send to server,
 *       else waiting for unlock and do the same things
 */

struct net *cl_net_start(char *address, int port, struct main_queue *m_queue)
{
	struct net *configure;
	struct sockaddr_in addr;
	int cl_sock;
	int net_pth_fd;
	pthread_t net_pth;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

	configure = malloc(sizeof(struct net));
	cl_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(cl_sock < 0) {
		perror("socket");
		return NULL;
	}
	
	configure->mutex = mutex;
	configure->m_queue = m_queue;

	configure->socket = cl_sock;
	configure->net_queue_head = NULL;

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(address);

	if(connect(cl_sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		perror("connect");
		return NULL;
	}

	net_pth_fd = pthread_create(&net_pth, NULL, net_work, configure);
	if(net_pth_fd != 0) {
		perror("pthred_create");
		return NULL;
	}
	configure->pthreadfd = net_pth;

	return configure;
}



void *net_work(void *arg)
{
	struct net *configure = (struct net*)arg;
	int max_sock;
	int msg_error;
	struct timeval timeout;
	fd_set readfd;
	fd_set writefd;
	fd_set errorfd;
	struct sigaction sigact;

	sigact.sa_handler = net_usr1_handler;

	if (sigaction(SIGUSR1, &sigact, NULL)) {
		perror("sigaction");
		exit(EXIT_FAILURE);
	}

	while(1) {
		FD_ZERO(&readfd);
		FD_ZERO(&writefd);
		FD_ZERO(&errorfd);
		FD_SET(configure->socket, &readfd);
		FD_SET(configure->socket, &writefd);
		FD_SET(configure->socket, &errorfd);

		timeout.tv_sec = 0;
		timeout.tv_usec = 1000;
		max_sock = configure->socket;

		if(select((max_sock + 1), &readfd, &writefd, &errorfd, &timeout) < 0) {
			perror("select");
			return NULL;
		}

		if(FD_ISSET(configure->socket, &readfd))
			cl_net_recv(configure);
		if(FD_ISSET(configure->socket, &writefd))
			if(cl_net_processing_event(configure) == -1)
				return NULL;
		if(FD_ISSET(configure->socket, &errorfd)) {
			msg_error = errno;
			cl_main_make_event(configure->m_queue, NET_ERROR,
			                   (void *)&msg_error, sizeof(msg_error));
			//return NULL;
		}
	}
	return NULL;
}



void cl_net_wait(struct net *configure)
{
	pthread_join(configure->pthreadfd, NULL);
}

/*
 * NAME: cl_net_send_placement
 *
 * IN:	 struct net *configure - configure which was filled by net_start and
 * 		main
 * 	 placement *net_placement - client ships placement
 *
 * OUT:	 nothing
 *
 * WORK: takes configure and net_placement to place it into the net queue
 */
void cl_net_send_placement(struct net *configure, placement net_placement) {
	struct net_queue *element = (struct net_queue *)malloc(
						sizeof (struct net_queue));

	element->net_next = NULL;
	element->type_msg = PLACEMENT;
	element->data = (void *)net_placement;
	element->data_len = sizeof (placement);
	cl_net_add_queue(configure, element);
}



/*
 * NAME: cl_net_send_shot
 *
 * IN:	 struct net *configure - configure which was filled by net_start and
 * 		main
 * 	 char shot_x - x coordinate of shot
 * 	 char shot_y - y coordinate of shot
 *
 * OUT:	 nothing
 *
 * WORK: takes configure and shot coordinates to place it into the net queue
 */
void cl_net_send_shot(struct net *configure, char shot_x, char shot_y) {
	struct net_queue *element = (struct net_queue *)malloc(
						sizeof (struct net_queue));
	char buff[2]; /* a char buffer for coordinates */

	buff[0] = shot_x;
	buff[1] = shot_y;
	element->net_next = NULL;
	element->type_msg = SHOT;
	element->data = (void *)buff;
	element->data_len = sizeof (char) * 2;
	cl_net_add_queue (configure, element);
}




/*
 * NAME: cl_net_send_nick
 *
 * IN:	 struct net *configure - configure which was filled by net_start and
 * 		main
 * 	 char *nick - string with a client nick
 * 	 int nick_len - nick length
 *
 * OUT:	 nothing
 *
 * WORK: takes configure and nick to place it into the net queue
 */
void cl_net_send_nick(struct net *configure, char *net_nick, int nick_len) {
	struct net_queue *element = (struct net_queue *)malloc(
						sizeof (struct net_queue));

	element->net_next = NULL;
	element->type_msg = NICK;
	element->data = (void *)net_nick;
	element->data_len = nick_len;
	cl_net_add_queue (configure, element);
}



/*
 * NAME: cl_net_send_error
 *
 * IN:	 struct net *configure - configure which was filled by net_start and
 * 		main
 * 	 int net_error - client error number
 *
 * OUT:	 nothing
 *
 * WORK: takes configure and net_error to place it into the net queue
 */
void cl_net_send_error(struct net *configure, int net_error) {
	struct net_queue *element = (struct net_queue *)malloc(
						sizeof (struct net_queue));

	element->net_next = NULL;
	element->type_msg = ERROR;
	element->data = (void *)&net_error;
	element->data_len = sizeof (int);
	cl_net_add_queue (configure, element);
}



/*
 * NAME: cl_net_send_end
 *
 * IN:	 struct net *configure - configure which was filled by net_start and
 * 		main
 *
 * OUT:	 nothing
 *
 * WORK: takes configure to place END into the net queue
 */
void cl_net_stop(struct net *configure) {
	struct net_queue *element = (struct net_queue *)malloc(
						sizeof (struct net_queue));

	element->net_next = NULL;
	element->type_msg = END;
	element->data = NULL;
	element->data_len = -1;
	cl_net_add_queue (configure, element);
}



/*
 * NAME: cl_net_add_queue
 *
 * IN:	 struct net_queue *net_queue_head - a pointer to the net queue head
 * 	 struct net_queue *element - a pointer to the added element of net
 * 		queue
 *
 * OUT:	 nothing
 *
 * WORK: if mutex is not locked then the element will be added to the net queue
 */
void cl_net_add_queue(struct net *configure, struct net_queue *element) {

	struct net_queue *idp; /* a queue iterator */

	pthread_mutex_lock(&configure->mutex);

	if(configure->net_queue_head == NULL) {
	// if the queue doesn't exists
		configure->net_queue_head = element;
	} else {
		// else
		for (idp = configure->net_queue_head; idp->net_next != NULL; idp = 
			 idp->net_next);
		idp->net_next = element;
	}

	pthread_mutex_unlock(&configure->mutex);
}



/*
 * NAME: cl_net_del_queue
 *
 * IN:	 struct net_queue *net_queue_head - a pointer to the net queue head
 *
 * OUT:	 nothing
 *
 * WORK: deletes the first element of the net queue
 */
void cl_net_del_queue(struct net *configure) {
	struct net_queue *idp; /* a queue iterator */

	if(configure->net_queue_head != NULL) {
	    idp = configure->net_queue_head;
	    configure->net_queue_head = configure->net_queue_head->net_next;
	    free(idp);
	}
}



/*
 * NAME: cl_net_recv
 *
 * IN:	 struct net *configure - configure which was filled by net_start and
 * 		main
 *
 * OUT:	 nothing
 *
 * WORK: receives information from net server module, takes configure to place
 * 	 received information into the main queue
 * 	 in case when recv returns an error then in main queue will be added
 * 	 an error number from errno or -1 if the server has closed connection
 */
void cl_net_recv(struct net *configure) {
	char buff[SIZE_BUF]; /* buffer for contain net information in recv */
	int recv_bytes; /* receive bytes */
	int err = 0; /* buffer for contain error. 0 = no error */
	int code; /* buffer for contain code of MAIN_EVENT_TYPES */
	int size; /* size of received data which we need to send in main */

	recv_bytes = recv(configure->socket, buff, SIZE_BUF, 0);
	//fprintf(stderr, "net: %d", (int)configure->m_queue->main_id);
	if (recv_bytes < 0) {
	// receive error
		//fprintf (stderr, "net recv < 0\n");
		err = errno;
		cl_main_make_event(configure->m_queue, NET_ERROR,
					(void *)&err, sizeof(err));
	} else if(!recv_bytes) {
		// server has closed a connection
		//fprintf (stderr, "net recv == 0\n");
		err = -1;
		cl_main_make_event(configure->m_queue, NET_ERROR,
					(void *)&err, sizeof(err));
	} else {
		// it's ok
		switch(buff[0]) {
			case START:
				if(recv_bytes != sizeof(char) * 2)
					err = -2;
				code = NET_START_GAME;
				size = sizeof(char);
				break;

			case SHOT:
				if(recv_bytes != sizeof(char) * 4)
					err = -2;
				code = NET_SHOT_RESULT;
				size = sizeof(char) * 3;
				break;

			case ERROR:
				if(recv_bytes != sizeof(char))
					err = -2;
				code = NET_ERROR;
				size = sizeof(int);
				break;

			case PLACEMENT:
				if(recv_bytes != (sizeof(char) + sizeof(placement)))
					err = -2;
				code = NET_PLACEMENT;
				size = sizeof(placement);
				break;
		}
		//fprintf(stderr, "err = %d\nsize = %d\nrecv_bytes = %d\n\n", err, size, recv_bytes);
		if(!err)
			cl_main_make_event(configure->m_queue, code,	(void *)(buff+1),
					size);
		else
			cl_main_make_event(configure->m_queue, NET_ERROR,
					(void *)&err, sizeof(err));
	}
}

void net_usr1_handler(int a)
{
}
