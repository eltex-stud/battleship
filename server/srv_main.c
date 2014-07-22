/* vim: set noexpandtab tabstop=4 shiftwidth=4 smartindent: */

#include <stdlib.h>
#include <stdio.h>

#include <server/srv_net.h>
#include <server/srv_logic.h>

enum turn { MY, ENEMY };

struct client_data {
	struct srv_net_client *client;
	struct srv_logic_map *map;
	struct client_data *enemy;
	enum turn turn;
};

struct client_list {
	struct client_data *client_data;
	struct client_list *next;

};

struct main_data {
	struct client_list *clients_data;
};

void *new_client(struct srv_net_client *client, void *main_data);
void placement_received(struct srv_net_client *client, placement plcmnt,
		void *client_data, void *main_data);
void del_client(struct srv_net_client *client, void *client_data,
		void *main_data);
void shot_received(struct srv_net_client *client, struct shot *shot,
		void *client_data, void *main_data);
void nick_received(struct srv_net_client *client, char *nick,
		void *client_data, void *main_data);


/**
 * @brief Start server
 *
 * @param argc number of command line args
 * @param argv[] command line args
 *
 * @return program return code
 */
int main(int argc, char *argv[])
{
	struct srv_net *net;
	struct main_data main_data;
	struct srv_net_client_ops client_ops;
	char *ip;
	int port;

	/* Check if usage is correct */
	if(argc != 3) {
		printf("Usage: %s ip port\n", argv[0]);
		return 1;
	}

	/* Retrieve ip and port from command line arguments */
	ip = argv[1];
	port = atoi(argv[2]);

	/* Init main_data */
	main_data.clients_data = NULL;

	/* Init client operations */
	client_ops.new_client = new_client;
	client_ops.placement_received = placement_received;
	client_ops.del_client = del_client;
	client_ops.shot_received = shot_received;
	client_ops.nick_received = nick_received;

	/* Run main loop */
	net = srv_net_init(ip, port);
	srv_net_wait_events(net, NULL, client_ops, &main_data);
	srv_net_del(net);

	return 0;
}


void shot_received(struct srv_net_client *client, struct shot *shot,
		void *client_data_void, void *main_data_void)
{
	enum shot_result result;
	struct client_data *client_data;
	struct client_data *enemy_data;
	placement my_placement;
	placement enemy_placement;

	client_data = client_data_void;
	enemy_data = client_data->enemy;

	/* Send error if not his turn */
	if(client_data->turn != MY) {
		srv_net_send_error(client, SRV_NET_ERR_NOT_YOUR_TURN);
		return;
	}

	/* Send error if malformed shot */
	shot_result = srv_logic_make_shot(enemy_data->map, shot);
	if(shot_result == SRV_LOGIC_BAD_SHOT) {
		srv_net_send_error(client, SRV_NET_ERR_BAD_SHOT);
		return;
	}

	/* Send shot result to each client */
	srv_net_send_shot_result(client, shot, shot_result);
	srv_net_send_shot_result(enemy_data->client, shot, shot_result);

	/* Switch turn */
	client_data->turn = ENEMY;
	enemy_data->turn = MY;

	/* If game finished */
	if(shot_result == SRV_SHOT_RESULT_END) {
		/* TODO: rethink. probably should store placement in main module */
		my_placement = srv_logic_map_to_placement(client_data->map);
		enemy_placement = srv_logic_map_to_placement(enemy_data->map);

		/* Unbind clients from each other (for correctness of del_client */
		enemy_data->enemy = NULL;
		client_data->enemy = NULL;

		/* Tell players their enemy placement */
		srv_net_send_placement(client, enemy_placement);
		srv_net_send_placement(enemy_data->client, my_placement);

		/* Disconnect clients */
		srv_net_del_client(client);
		srv_net_del_client(enemy_data->client);
	}
}


void nick_received(struct srv_net_client *client, char *nick,
		void *client_data, void *main_data)
{
	/* Do nothing for now */
}


void *new_client(struct srv_net_client *client, void *main_data)
{
	struct main_data *m_data;
	m_data = main_data;
	
	struct client_list *cl_list;
	cl_list = (struct client_list*)malloc(sizeof(cl_list));
	
	struct client_data *cl_data;
	cl_data = (struct client_data*)malloc(sizeof(cl_data));
	cl_data->client = client;
	cl_data->map = NULL;
	cl_data->enemy = NULL;
	cl_list->client_data = cl_data;
	cl_list->next = NULL;

	if(m_data->clients_data == NULL){
		m_data->clients_data = cl_list;
	} else {
		struct client_list *temp;
		temp = m_data->clients_data;
		while(temp->next != NULL){
			temp = temp->next;
		}
		temp->next = cl_list;
	}
	return cl_data;
}


void del_client(struct srv_net_client *client, void *client_data,
		void *main_data)
{
	struct client_list *temp, *prev;
	struct main_data *m_data;
	m_data = main_data;
	temp = m_data->clients_data;
	prev = NULL;

	if(temp->client_data == (struct client_data*)client_data)
	{
		free(temp->client_data);
		m_data->clients_data = temp->next;
		if(temp->next == NULL){
			m_data->clients_data = NULL;
		}
		free(temp);
	} else {
		while(temp->client_data != (struct client_data*)client_data){
			prev = temp;
			temp = temp->next;
		}
		free(temp->client_data);
		if(temp->next != NULL){
			prev->next = temp->next;
		} else {
			prev->next = NULL;
		}
		free(temp);
	}
}
