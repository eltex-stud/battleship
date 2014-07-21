/* vim: set noexpandtab tabstop=4 shiftwidth=4 smartindent: */
#ifndef BATTLESHIP_SRV_NET_H_
#define BATTLESHIP_SRV_NET_H_

/**
 * @struct srv_net_client_ops
 *
 * @brief Set of callbacks to handle network events
 */
struct srv_net_client_ops {
	/**
	 * @brief Function to handle player connections
	 *
	 * @param client connected client
	 * @param main_data data passed in srv
	 *
	 * @return data specific to this client
	 */
	void *(*new_client)(struct srv_net_client *client, void *main_data);

	/**
	 * @brief Function to handle client placement receiving
	 *
	 * @param client client who sent the placement
	 * @param placement placement of client's ships
	 * @param client_data data returned from new_client
	 * @param main_data data passed to srv_net_wait_events
	 */
	void (*placement_received)(struct srv_net_client *client, placement plcmnt,
			void *client_data, void *main_data);

	/**
	 * @brief Function to handle client disconnect. Should free resources
	 * allocated in new_client.
	 *
	 * @param client disconnected client
	 * @param client_data data returned from new_client
	 * @param main_data data passed to srv_net_wait_events
	 */
	void (*del_client)(struct srv_net_client *client, void *client_data,
			void *main_data);

	/**
	 * @brief Function to handle shots
	 *
	 * @param client client who sent the shot
	 * @param shot the shot that player did
	 * @param client_data data returned from new_client call
	 * @param main_data data passed to srv_net_wait_events
	 */
	void (*shot_received)(struct srv_net_client *client, struct shot *shot,
			void *client_data, void *main_data);

	/**
	 * @brief Function to handle client nick receiving
	 *
	 * @param client client who sent the shot
	 * @param nick the nick
	 * @param client_data data returned from new_client call
	 * @param main_data data passed to srv_net_wait_events
	 */
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
