/* vim: set noexpandtab tabstop=4 shiftwidth=4 smartindent: */

#include "cl_net.h"

int net_start(char address, int port)
{
	int cl_sock;
	int net_pth_fd;
	pthread_t net_pth;
	cl_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(cl_sock < 0) {
		perror("socket");
		return error;
	}
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(address);

	if(connect(cl_sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		perror("connect");
		return error;
	}

	net_pth_fd = pthread_create(&net_pth, NULL, net_work, cl_sock);
	if(net_pth_fd != 0) {
		perror("pthred_create");
		return error;
	}
	return error;
}

void *net_work(void *arg)
{
    int loc_sock = *(int*)arg, i;
    while(1){
	i = recv(loc_sock, buf, sizeof(message), 0);
	if(i <= 0){
	    break;
	}
	printf("%s \n", buf);
    }
    return 0;
}

