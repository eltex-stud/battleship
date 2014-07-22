/* vim: set noexpandtab tabstop=4 shiftwidth=4 smartindent: */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#include <net_types.h>
#include "srv_net.h"

#define SIZE_BUFF 1024


struct srv_net_queue {
	int fd;
	int size;
	int index;

	char data[127];
	struct srv_net_queue *next;
};

struct srv_net_network {
	int fd;
	void *main_data;
	struct srv_net_queue *queue;
};

struct srv_net_client {
	int fd;
	void *client_data;
	struct srv_net_network *network;
};

struct srv_net_client client_list[32];
struct srv_net_client_ops main_client_ops;

struct srv_net_client *srv_net_find_client(int fd)
{
	int i;

	for(i=0; i<=32; i++) {
		if(client_list[i].fd == fd) {
			return &(client_list[i]);
		}
	}
	return NULL;
}

struct srv_net_queue *head = NULL;
struct srv_net_queue *tail = NULL;

struct srv_net_network *srv_net_start(char *ip, short int port)
{
	int server_sock;

	struct sockaddr_in server_addr;
	struct srv_net_network *network;
	network = (struct srv_net_network *)malloc(sizeof(struct srv_net_network));
	network->fd = 0;

	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(server_sock < 0) {
		printf("Error create socket");
		return 0;
	}

	if(fcntl(server_sock, F_SETFL, O_NONBLOCK) < 0) {
		printf ("Error set nonblock socket");
		return 0;
	}

	memset(&server_addr, 0, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip);
	server_addr.sin_port = htons(port);

	if((bind(server_sock,(struct sockaddr *)&server_addr,
			sizeof(server_addr))) < 0) {
		perror("Error bind");
		return 0;
	}

	network->fd = server_sock;
	return network;
}

int srv_net_stop(struct srv_net_network *net)
{
  close(net->fd);
  free(net);
  return 0;
}

void srv_net_wait_events(struct srv_net_network *net, int *clients[] __attribute__((unused)),
		struct srv_net_client_ops client_ops, void *main_data)
{
	int idx;
	int jdx;
	int kdx;

	int size;
	int size_send;

	main_client_ops = client_ops;

	struct srv_net_queue *temp;
	struct srv_net_queue *temp2;

	char placement[100];
	char buff[128];
	char nick[127];

	unsigned char *tmsg = (unsigned char *)buff;
	struct srv_net_shot *cl_shot;
	struct timeval timeout;
	struct srv_net_client *client;

	kdx=0;
	idx=0;
	jdx=0;

	size=0;
	size_send=0;

	net->main_data = main_data;

	listen(net->fd, 5);

	while(1) {
		fd_set readset;
		fd_set writeset;

		FD_ZERO(&readset);
		FD_ZERO(&writeset);

		FD_SET(net->fd, &readset);


		for(idx=0; idx<32; idx++) {
			if(client_list[idx].fd!=0) {
				FD_SET(client_list[idx].fd, &readset);
			}
		}

		if(head != NULL) {
			for(temp = head->next; temp->next!=NULL; temp=temp->next) {
				FD_SET (temp->fd, &writeset);
			}
		}

		timeout.tv_sec=1;
		timeout.tv_usec=0;

		if(select(33, &readset, &writeset, NULL, &timeout) <= 0) {
			printf("Error select");
		}

		if(FD_ISSET(net->fd, &readset)) {
			client_list[idx].fd = accept(net->fd, NULL, NULL);
			fcntl(client_list[idx].fd, F_SETFL, O_NONBLOCK);

			client_list[idx].client_data = main_client_ops.new_client(&(client_list[idx]), net->main_data);
			
			idx++;
		}

		for(jdx=0; jdx<32; idx++) {
			if(client_list[jdx].fd!=0) {
				if(FD_ISSET(client_list[jdx].fd, &readset)) {
					size = recv(client_list[jdx].fd, &buff, SIZE_BUFF, 0);
					switch((enum types_msg)tmsg[0]) {
						case NICK:
							for(kdx = 1; kdx <= size; kdx++) {
								nick[kdx-1] = buff[kdx];
							}
							client = srv_net_find_client(client_list[jdx].fd);
							main_client_ops.nick_received(client, nick,
									client_list[idx].client_data, net->main_data);
							break;

						case PLACEMENT:
							for(kdx = 1; kdx <= 101; kdx++) {
								placement[kdx-1] = buff[kdx];
							}
							client = srv_net_find_client(client_list[jdx].fd);
							main_client_ops.placement_received(client, placement,
									client_list[idx].client_data, net->main_data);
							break;

						case SHOT:
							cl_shot = (struct srv_net_shot *)malloc(sizeof(struct srv_net_shot));
							cl_shot->x = (int)buff[1];
							cl_shot->y = (int)buff[2];
							client = srv_net_find_client(client_list[jdx].fd);
							main_client_ops.shot_received(client, cl_shot,
									client_list[idx].client_data, net->main_data);
							break;

						case END:
							client = srv_net_find_client(client_list[jdx].fd);
							main_client_ops.del_client(client,
									client_list[idx].client_data, net->main_data);
							break;
						case ERROR:
						case START:
						default:
							printf("Client sent something wrong\n");
					}
				}
			}
		}
		if(head != NULL) {
			for(temp = head->next; temp->next!=NULL; temp=temp->next) {
				if(FD_ISSET (temp->fd, &writeset)) {
					size_send = send (temp->fd, temp->data, temp->size, 0); //WARNING!!! ADD SEND FROM INDEX!!!
					if((size_send+temp->index) < temp->size) {
						temp->index+=size_send;
					}
					if((size_send+temp->index) == temp->size) {
						for(temp2 = head->next; temp2->next!=temp; temp2=temp2->next);
						temp2->next = temp->next;
						free(temp);
					}
				}
			}
		}

	}
}

int srv_net_del_client(struct srv_net_client *client)
{
	int idx;

	close(client->fd);

	main_client_ops.del_client(client, client->client_data,
			client->network->main_data);

	for(idx=0; idx<32; idx++) {
		if(client->fd == client_list[idx].fd) {
			free((client_list[idx].network)->queue);
			free(client_list[idx].network);
			client_list[idx].fd=0;
			client_list[idx].network=0;
		}
	}
	return 0;
}

int srv_net_send_game_start (struct srv_net_client *client, enum srv_net_turn r)
{
	char buff[2];
	struct srv_net_queue *queue;

	queue = (struct srv_net_queue *)malloc(sizeof(struct srv_net_queue));

	buff[0] = START;
	buff[1] = r;

	memcpy(queue->data, buff, 2);
	queue->size = sizeof(buff);
	queue->fd = client->fd;
	queue->index = 0;
	queue->next = NULL;

	if(client->network->queue == NULL) {
		client->network->queue = (struct srv_net_queue *)malloc(sizeof(struct srv_net_queue));
		head = client->network->queue;
		tail = head;

		tail->next = queue;
		tail = queue;
	}
	else{
		tail->next = queue;
		tail = queue;
	}

	return 0;
}

int srv_net_send_game_end(struct srv_net_client *client,
		enum srv_net_shot_result r)
{
	char buff[4];
	struct srv_net_queue *queue;

	queue = (struct srv_net_queue *)malloc(sizeof(struct srv_net_queue));

	buff[0] = SRV_NET_END;
	buff[3] = r;

	memcpy(queue->data, buff, 4);
	queue->size = sizeof(buff);
	queue->fd = client->fd;
	queue->index = 0;
	queue->next = NULL;

	if(client->network->queue == NULL) {
		client->network->queue = (struct srv_net_queue *)malloc(sizeof(struct srv_net_queue));
		head = client->network->queue;
		tail = head;

		tail->next = queue;
		tail = queue;
	}
	else{
		tail->next = queue;
		tail = queue;
	}

	return 0;
}
int srv_net_send_shot_result(struct srv_net_client *client,
		struct srv_net_shot *shot, enum srv_net_shot_result r)
{
	char buff[4];
	struct srv_net_queue *queue;

	queue = (struct srv_net_queue *)malloc(sizeof(struct srv_net_queue));

	buff[0] = SHOT;
	buff[1] = shot->x;
	buff[2] = shot->y;
	buff[3] = r;

	memcpy(queue->data, buff, 4);
	queue->size = sizeof(buff);
	queue->fd = client->fd;
	queue->index = 0;
	queue->next = NULL;

	if(client->network->queue == NULL) {
		client->network->queue = (struct srv_net_queue *)malloc(sizeof(struct srv_net_queue));
		head = client->network->queue;
		tail = head;

		tail->next = queue;
		tail = queue;
	}
	else{
		tail->next = queue;
		tail = queue;
	}

	return 0;
}

int srv_net_send_placement(struct srv_net_client *client, char *placement)
{
	char buff[101];
	struct srv_net_queue *queue;

	queue = (struct srv_net_queue *)malloc(sizeof(struct srv_net_queue));

	buff[0] = PLACEMENT;
	memcpy(buff+1, placement, 100);

	memcpy(queue->data, buff, 101);
	queue->size = sizeof(buff);
	queue->fd = client->fd;
	queue->index = 0;
	queue->next = NULL;

	if(client->network->queue == NULL) {
		client->network->queue = (struct srv_net_queue *)malloc(sizeof(struct srv_net_queue));
		head = client->network->queue;
		tail = head;

		tail->next = queue;
		tail = queue;
	}
	else {
		tail->next = queue;
		tail = queue;
	}
	return 0;
}

int srv_net_send_error(struct srv_net_client *client,
		enum srv_net_error_msg er_msg)
{
	char buff[2];
	struct srv_net_queue *queue;

	queue = (struct srv_net_queue *)malloc(sizeof(struct srv_net_queue));

	buff[0] = ERROR;
	buff[1] = er_msg;

	memcpy(queue->data, buff, 2);
	queue->size = sizeof(buff);
	queue->fd = client->fd;
	queue->index = 0;
	queue->next = NULL;

	if(client->network->queue == NULL) {
		client->network->queue = (struct srv_net_queue *)malloc(sizeof(struct srv_net_queue));
		head = client->network->queue;
		tail = head;

		tail->next = queue;
		tail = queue;
	}
	else{
		tail->next = queue;
		tail = queue;
	}
	return 0;
}
