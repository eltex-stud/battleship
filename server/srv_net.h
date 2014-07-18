/* vim: set noexpandtab tabstop=4 shiftwidth=4 smartindent: */
#ifndef BATTLESHIP_SRV_NET_H_
#define BATTLESHIP_SRV_NET_H_

struct srv_net_client_ops {
	void *new_client(struct srv_net_client *client, void *main_data);
	void placement_received(struct srv_net_client *client, void *client_data,
			void *main_data);
	void del_client(struct srv_net_client *client, void *client_data,
			void *main_data);
	void move_received(struct srv_net_client *client, void *client_data,
			void *main_data);
	void nick_received(struct srv_net_client *client, void *client_data,
			void *main_data);
};

#endif // BATTLESHIP_SRV_NET_H_
