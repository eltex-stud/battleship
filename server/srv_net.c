/* vim: set noexpandtab tabstop=4 shiftwidth=4 smartindent: */

#include "../include/srv_net.h"
#include "srv_net.h"

struct srv_net_queue {
	struct srv_net_queue *next;
}

struct srv_net_network {
	int fd;
	struct srv_net_queue *queue;
};

struct srv_net_client {
	int fd;
	struct srv_net_network *network;
};

struct srv_net_network *srv_net_start(char *ip, short int port){
	int server_sock;

	struct sockaddr_in server_addr;
	struct srv_net_network network;
	network.fd = 0;

	server_sock = socket (AF_INET, SOCK_STREAM, 0);
	if (server_sock < 0){
		printf ("Error create socket");
		return network;
	}

	if(fcntl (server_socket, F_SETFL, O_NONBLOCK) < 0){
		printf ("Error set nonblock socket");
		return network;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip);
	server_addr.sin_addr.s_addr = htons(port);

	if ((bind(server_sock, (struct sockaddr *)&server_addr,
			sizeof(server_addr))) < 0){
		printf ("Error bind");
		return network;
	}

	network.fd = server_socket;
	return network;
}
