/* vim: set noexpandtab tabstop=4 shiftwidth=4 smartindent: */
/*
 *The file cl_net.c/
 *
 */

#include "cl_net.h"

void cl_net_processing_event(struct net *configure)
{
	struct net_queue *idx; /* iterator struct net_queue */
	char buf[SIZE_BUF];
	pthread_mutex_lock(configure->mutex);
	idx = configure->net_queue_head;
	memcpy(buf, idx->type_msg, 1);
	memcpy(buf + 1, idx->data, idx->data_len);
	send(configure->socket, buf, idx->data_len + 1, 0);
	net_del_queue(configure->net_queue_head);
	pthread_mutex_unlock(configure->mutex);

};

struct net *net_start(char address, int port, struct main_queue *m_queue)
{
	struct net *configure;
	int cl_sock;
	int net_pth_fd;
	pthread_t net_pth;
	pthread_mutex_t *mutex = PTHREAD_MUTEX_INITIALIZER;

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
	addr.sin_addr.s_addr = htonl(address);

	if(connect(cl_sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		perror("connect");
		return NULL;
	}

	net_pth_fd = pthread_create(&net_pth, NULL, net_work, configure);
	if(net_pth_fd != 0) {
		perror("pthred_create");
		return NULL;
	}

	return configure;
}

void *net_work(void *arg)
{
    //int loc_sock = *(int*)arg;
	struct net *configure = (struct net*)arg;
	int byte_read;
	int max_sock;
	char buf_recv[SIZE_BUF];
	char *msg_error = "Net disconnect!";
	struct timeval timeout;
	fd_set readfd;
	fd_set writefd;
	fd_set errorfd;

    while(1){
		FD_ZERO(&readfd);
		FD_ZERO(&writefd);
		FD_ZERO(&errorfd);
		FD_SET(configure->socket, &readfd);
		FD_SET(configure->socket, &writefd);
		FD_SET(configure->socket, &errorfd);

		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		max_sock = configure->socket;

		if(select((max_sock + 1), &readfd, &writefd, &errorfd, &timeout) < 0) {
			perror("select");
			return ;
		}

		if(FD_ISSET(configure->socket, &readfd))
			net_recv(configure);
		if(FD_ISSET(configure->socket, &writefd))
			cl_net_processing_event(configure);
		if(FD_ISSET(configure->socket, &errorfd))
			cl_main_make_event(configure->m_queue, NET_ERROR, (void*)msg_error,
			                   sizeof(msg_error));
		}

}

