/* vim: set noexpandtab tabstop=4 shiftwidth=4 smartindent: */
#ifndef BATTLESHIP_SRV_MAIN_H_
#define BATTLESHIP_SRV_MAIN_H_

#include <server/srv_net.h>

enum srv_main_turn { MY, ENEMY };

struct srv_main_client_data {
	struct srv_main_client_data *enemy;
	struct srv_net_client *client;
	struct srv_logic_map *map;
	enum srv_main_turn turn;
};

struct srv_main_data_list {
	struct srv_main_client_data *client_data;
	struct srv_main_data_list *next;

};

struct srv_main_data {
	struct srv_main_data_list *clients_data;
};

void *srv_main_new_client(struct srv_net_client *client, void *main_data);
void srv_main_placement_received(struct srv_net_client *client,
		void *client_data, void *main_data);
void srv_main_del_client(struct srv_net_client *client, void *client_data,
		void *main_data);
void srv_main_move_received(struct srv_net_client *client, void *client_data,
		void *main_data);
void srv_main_nick_received(struct srv_net_client *client, void *client_data,
		void *main_data);

#endif // BATTLESHIP_SRV_MAIN_H_
