/* vim: set noexpandtab tabstop=4 shiftwidth=4 smartindent: */

#ifndef CL_MAIN
#define CL_MAIN

#include "cl_net.h"
#include "cl_gui.h"
#include "cl_logic.h"
#include "client.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>

#define ADDRESS_LENGTH 16

struct main_event {
	void *event_data;
	struct main_event *next;
	int data_length;
	char event_type;
};

struct main_queue {
	pthread_t main_id;
	pthread_mutex_t mutex;
	int net_working;
	struct main_event *head;
};

int cl_main_get_options(int argc, char *argv[], int *port, char address[]);
void cl_main_control(struct main_queue *queue, map my_map, map enemy_map,
                     struct net *network, struct gui *cl_gui);
void cl_main_check_shot(void * event_data, map my_map, struct net *network,
                        struct gui *cl_gui, enum player_state *turn);
void cl_main_check_net_shot(void *event_data, enum player_state *turn,
                            map my_map, map enemy_map, struct gui *cl_gui);
void cl_main_send_placement(void *event_data, struct main_queue *queue, map enemy_map,
                            struct gui *cl_gui, enum player_state turn, struct net *cl_net);
void cl_main_start_game(void *event_data, map my_map, enum player_state *turn,
                        struct gui *cl_gui);
struct main_event *cl_main_copy_queue(struct main_queue *queue);
void cl_main_clear_queue(struct main_event *head);
void usr1_handler(int a);

#endif
