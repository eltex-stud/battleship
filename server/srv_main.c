/* vim: set noexpandtab tabstop=4 shiftwidth=4 smartindent: */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <net_types.h>
#include <server/srv_net.h>
#include <server/srv_logic.h>

enum turn { MY, ENEMY };

enum client_connect_status {
	NICK_RECEIVED = 1,
	PLACEMENT_RECEIVED = 2,
	HAS_ENEMY = 4,
	READY_TO_START = 7,
	GAME_STARTED = 8,
	GAME_ENDED = 9
};

struct client_data {
	struct srv_net_client *client;
	struct srv_logic_map *map;
	struct client_data *enemy;
	enum turn turn;
	char plcmnt[10][10];
	int client_status;
};

struct client_list {
	struct client_data *client_data;
	struct client_list *next;

};

struct main_data {
	struct client_list *clients_data;
};

static void *new_client(struct srv_net_client *client, void *main_data);
static void placement_received(struct srv_net_client *client, char *plcmnt,
		void *client_data, void *main_data);
static void del_client(struct srv_net_client *client, void *client_data,
		void *main_data);
static void shot_received(struct srv_net_client *client,
		struct srv_net_shot *shot, void *client_data, void *main_data);
static void nick_received(struct srv_net_client *client, char *nick,
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
	struct srv_net_network *net;
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
	port = strtol(argv[2], NULL, 10);
	if(!(port > 0 && port < 65535)) {
		fprintf(stderr, "Invalid port number");
		return 1;
	}

	/* Init main_data */
	main_data.clients_data = NULL;

	/* Init client operations */
	client_ops.new_client = new_client;
	client_ops.placement_received = placement_received;
	client_ops.del_client = del_client;
	client_ops.shot_received = shot_received;
	client_ops.nick_received = nick_received;

	/* Run main loop */
	net = srv_net_start(ip, port);
	if(net == NULL) {
		fprintf(stderr, "Network start error\n");
		return 1;
	}
	srv_net_wait_events(net, NULL, client_ops, &main_data);
	srv_net_stop(net);

	return 0;
}

static void send_game_start(struct client_data *cl_data)
{
	if(cl_data->turn == MY){
		srv_net_send_game_start(cl_data->client, SRV_NET_YOUR_TURN);
		srv_net_send_game_start(cl_data->enemy->client, SRV_NET_ENEMY_TURN);
	} else {
		srv_net_send_game_start(cl_data->client, SRV_NET_ENEMY_TURN);
		srv_net_send_game_start(cl_data->enemy->client, SRV_NET_YOUR_TURN);
	}
	cl_data->client_status = GAME_STARTED;
	cl_data->enemy->client_status = GAME_STARTED;
	printf("game started\n");
}

static enum srv_net_shot_result logic_to_net_shot_result(enum srv_logic_shot_result r)
{
	switch (r) {
		case SRV_LOGIC_RESULT_HIT:
			return SRV_NET_HIT;
		case SRV_LOGIC_RESULT_KILL:
			return SRV_NET_KILL;
		case SRV_LOGIC_RESULT_MISS:
			return SRV_LOGIC_RESULT_MISS;
		case SRV_LOGIC_RESULT_END_GAME:
			return SRV_NET_END;
		default:
			assert(0 && "Programming error: bad srv_logic_shot_result");
	}
}

static void shot_received(struct srv_net_client *client,
		struct srv_net_shot *shot, void *client_data_void,
		void *main_data_void __attribute__((unused)))
{
	enum srv_logic_shot_result shot_result;
	enum srv_net_shot_result net_shot_result;
	struct client_data *client_data;
	struct client_data *enemy_data;
	char *my_placement;
	char *enemy_placement;
	struct srv_logic_shot logic_shot;
	printf("shot received %d %d\n", shot->x, shot->y);

	client_data = client_data_void;
	enemy_data = client_data->enemy;

	/* Send error if not his turn */
	if(client_data->turn != MY) {
		srv_net_send_error(client, SRV_NET_NOT_YOUR_TURN);
		return;
	}

	/* Send error if malformed shot */
	if (shot->x < 0 || shot->x >= 10 || shot->y < 0 || shot->y >= 10) {
		srv_net_send_error(client, SRV_NET_BAD_SHOT);
		return;
	}
	logic_shot.x = shot->x;
	logic_shot.y = shot->y;
	shot_result = srv_logic_make_shot(enemy_data->map, &logic_shot);

	/* Send shot result to each client */
	net_shot_result = logic_to_net_shot_result(shot_result);
	srv_net_send_shot_result(client, shot, net_shot_result);
	srv_net_send_shot_result(enemy_data->client, shot, net_shot_result);

	/* Switch turn */
	client_data->turn = ENEMY;
	enemy_data->turn = MY;

	/* If game finished */
	if(shot_result == SRV_LOGIC_RESULT_END_GAME) {
		my_placement = (char *)client_data->plcmnt;
		enemy_placement = (char *)enemy_data->plcmnt;

		/* Unbind clients from each other (for correctness of del_client */
		enemy_data->enemy = NULL;
		client_data->enemy = NULL;

		/* Tell players their enemy placement */
		srv_net_send_placement(client, enemy_placement);
		srv_net_send_placement(enemy_data->client, my_placement);

		client_data->client_status = GAME_ENDED;
		enemy_data->client_status = GAME_ENDED;

		/* Disconnect clients */
		srv_net_del_client(client);
		srv_net_del_client(enemy_data->client);
		printf("shot received end game end\n");
	}
	printf("shot received end\n");
}


static void nick_received(struct srv_net_client *client __attribute__((unused)),
		char *nick /*__attribute__((unused))*/,
		void *client_data __attribute__((unused)),
		void *main_data __attribute__((unused)))
{
	struct client_data *cl_data = client_data;

	cl_data->client_status |= NICK_RECEIVED;

	if(cl_data->client_status == READY_TO_START
			&& cl_data->enemy->client_status == READY_TO_START) {
		send_game_start(cl_data);
	}

	printf("nick received %s\n", nick);
	printf("%s\n", nick);
}


static void *new_client(struct srv_net_client *client, void *main_data)
{
	struct main_data *m_data;
	struct client_list *cl_list;
	struct client_data *cl_data;
	struct client_list *temp;

	m_data = main_data;

	cl_list = (struct client_list*)malloc(sizeof(struct client_list));
	cl_data = (struct client_data*)malloc(sizeof(struct client_data));

	cl_data->client = client;
	cl_data->map = NULL;
	cl_data->enemy = NULL;
	cl_data->client_status = 0;
	
	cl_list->client_data = cl_data;
	cl_list->next = NULL;

	if(m_data->clients_data == NULL){
		m_data->clients_data = cl_list;
	} else {
		temp = m_data->clients_data;
		while(temp->next != NULL){
			temp = temp->next;
		}
		temp->next = cl_list;
	}

	temp = m_data->clients_data;
	while(temp->next != NULL){
		if(temp->client_data->enemy == NULL){
			temp->client_data->enemy = cl_data;
			temp->client_data->turn = ENEMY;
			cl_data->enemy = temp->client_data;
			cl_data->turn = MY;
			cl_data->client_status |= HAS_ENEMY;
			cl_data->enemy->client_status |= HAS_ENEMY;
			break;
		}
		temp = temp->next;
	}

	return cl_data;
}

static void del_client(struct srv_net_client *client __attribute__((unused)),
		void *client_data __attribute__((unused)), void *main_data)
{
	struct client_list *temp, *prev;
	struct main_data *m_data;
	struct client_data *enemy_data;

	printf("del client\n");
	
	m_data = main_data;
	temp = m_data->clients_data;
	prev = NULL;

	if(temp->client_data == (struct client_data*)client_data){
		enemy_data = temp->client_data->enemy;
		free(temp->client_data);
		m_data->clients_data = temp->next;
		free(temp);
	} else {
		while(temp->client_data != (struct client_data*)client_data){
			prev = temp;
			temp = temp->next;
		}
		enemy_data = temp->client_data->enemy;
		free(temp->client_data);
		prev->next = temp->next;
		free(temp);
	}

	if (enemy_data != NULL) {
		enemy_data->enemy = NULL;
		srv_net_del_client(enemy_data->client);
	}

	printf("return from del client\n");
}


static void placement_received(struct srv_net_client *client __attribute__((unused)),
		char *plcmnt, void *client_data, void *main_data __attribute__((unused)))
{
	printf("start placement_received\n");
	struct client_data *cl_data;
	cl_data = client_data;
	char map[10][10];
	int col;
	int row;
	int idx;
	idx = 0;

	for(row = 0; row < 10; row++){
		for(col = 0; col < 10; col++){
			map[row][col] = plcmnt[idx];
			cl_data->plcmnt[row][col] = plcmnt[idx];
			idx++;
		}
	}
	cl_data->map = srv_logic_placement_to_map(map);

	cl_data->client_status |= PLACEMENT_RECEIVED;

	if(cl_data->client_status == READY_TO_START
			&& cl_data->enemy->client_status == READY_TO_START) {
		send_game_start(cl_data);
	}

	printf("placement_received\n");
}
