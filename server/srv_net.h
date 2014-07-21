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

enum srv_net_shot_result {
	SRV_NET_MISS,
	SRV_NET_HIT,
	SRV_NET_KILL,
	SRV_NET_END
};

enum srv_net_error_msg {
	SRV_NET_BAD_SHOT,
	SRV_NET_NOT_SHOT
};

struct srv_net_shot {
	int x;
	int y;
};

struct srv_net_network *srv_net_start(char *ip, short int port);
int srv_net_stop(struct srv_net_network *net);
int srv_net_send_shot_result(struct srv_net_client *client,
		struct srv_net_shot *shot, enum result r);
int srv_net_send_game_start(struct srv_net_client *client, int turn);
int srv_net_send_placement(struct srv_net_client *client, char **placement);
int srv_net_send_game_end(struct srv_net_client *client,
		enum srv_net_shot_result r);
int srv_net_send_error(struct srv_net_client *client,
		enum srv_net_error_msg er_msg);
int srv_net_del_client(struct srv_net_client *client);
void srv_net_wait_events(struct srv_net_network *net, int *clients[],
		struct srv_net_client_ops client_ops, void *main_data);
#endif // BATTLESHIP_SRV_NET_H_
