/* vim: set noexpandtab tabstop=4 shiftwidth=4 smartindent: */
#ifndef BATTLESHIP_SRV_NET_H_
#define BATTLESHIP_SRV_NET_H_

struct srv_net_client_ops {
	void *(*new_client)(struct srv_net_client *client, void *main_data);
	void (*placement_received)(struct srv_net_client *client, placement plcmnt,
			void *client_data, void *main_data);
	void (*del_client)(struct srv_net_client *client, void *client_data,
			void *main_data);
	void (*shot_received)(struct srv_net_client *client, struct shot *shot,
			void *client_data, void *main_data);
	void (*nick_received)(struct srv_net_client *client, char *nick,
			void *client_data, void *main_data);
};

int srv_net_start(char ip, short int port);
int srv_net_stop(int net);

void srv_net_wait_events(int net, int *clients[],
		struct srv_net_client_ops client ops, *main_data);

int srv_net_send_shot_result(int client, struct shot, enum result r);
int srv_net_send_game_start(int client, int turn);
int srv_net_send_placement(int client, char **placement);
int srv_net_send_game_end(int client, enum result r);
int srv_net_send_error(int client, enum error_msg er_msg);

int srv_net_del_client(int client);
#endif // BATTLESHIP_SRV_NET_H_
