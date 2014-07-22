/* vim: set noexpandtab tabstop=4 shiftwidth=4 smartindent: */


#include "../include/srv_net.h"
#include "srv_net.h"

struct srv_net_queue {
	struct srv_net_queue *next;
};

struct srv_net_network {
	int fd;
	struct srv_net_queue *queue;
};

struct srv_net_client {
	int fd;
	struct srv_net_network *network;
};

struct srv_net_client client_list[32];

struct srv_net_network *srv_net_start(char *ip, short int port)
{
	int server_sock;

	struct sockaddr_in server_addr;
	struct srv_net_network *network;
	network = (struct srv_net_network *)malloc(sizeof(struct srv_net_network));
	network.fd = 0;

	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(server_sock < 0) {
		printf("Error create socket");
		return 0;
	}

	if(fcntl(server_socket, F_SETFL, O_NONBLOCK) < 0) {
		printf ("Error set nonblock socket");
		return 0;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip);
	server_addr.sin_addr.s_addr = htons(port);

	if((bind(server_sock,(struct sockaddr *)&server_addr,
			sizeof(server_addr))) < 0) {
		printf("Error bind");
		return 0;
	}

	network.fd = server_socket;
	return network;
}


int srv_net_stop(struct srv_net_client *net)
{
  close(net->fd);
  free(net);
}


void srv_net_wait_events(struct srv_net_network *net, int *clients[],
		struct srv_net_client_ops client_ops, void *main_data)
{
	int i;
	int j;

	char buff[128];
	unsigned char *tmsg = buff;

	struct timeval timeout;

	idx=0;
	jdx=0;

	listen(net->fd, 5);

	while(1) {
		fd_set readset;
		FD_ZERO(&readset);
		FD_SET(net->fd, &readset);

		for(idx=0; idx<=32; idx++) {
			if(client_list[idx].fd!=0) {
				FD_SET(client_list[idx].fd, &readset);
			}
		}

		timeout.tv_sec=10;
		timeout_tv_usec=0;

		if(select(33, &readset, NULL, NULL, &timeout) <= 0) {
			printf("Error select");
		}

		if(FD_ISSET(net->fd, &readset)) {
			client_list[idx].fd = accept(*net.fd, NULL, NULL);
			fcntl(client_list[idx].fd, F_SETFL, O_NONBLOCK);

			srv_main_new_client (client_list[idx], *main_data);
			idx++;
		}

		for(jdx=0; jdx<=32; idx++) {
			if(client_list[jdx].fd!=0) {
				if(FD_ISSET(client_list[jdx].fd, &readset)) {
					recv(client_list[jdx].fd, &buff, SIZE_BUFF, 0);
					switch((enum types_msg *)tmsg) {
						case NICK:

							break;

						case PLACEMENT:
							break;

						case SHOT:
							break;

						case ERROR
							break;
					}
				}
			}
		}
	}
}
