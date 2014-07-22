/* vim: set noexpandtab tabstop=4 shiftwidth=4 smartindent: */

#include "cl_main.h"



int main(int argc, char *argv[])
{
	int srv_port; /* Server address */
	char srv_address[ADDRESS_LENGTH]; /* Server port */
	struct main_event cap = { NULL }, *head = &cap;
	struct main_queue queue = { pthread_self(), PTHREAD_MUTEX_INITIALIZER, 
	                            head };
	struct net *cl_net = NULL;
	struct gui *cl_gui = NULL;
	map cl_map, cl_enemy_map;
	placement cl_placement;

	if (cl_main_get_options(argc, argv, &srv_port, srv_address)) {
		printf("Illegal command line options.\n");
		printf("Use \"-a\" for address and \"-p\" for port.\n");
		exit(EXIT_FAILURE);
	}

	if ((cl_net = cl_net_start(srv_address, srv_port, &queue)) == NULL) {
		printf("Server is unreachable.\n");
		exit(EXIT_FAILURE);
	}

	if ((cl_gui = cl_gui_start(&queue)) == NULL) {
		printf("Can't initialize ncurses interface\n");
		exit(EXIT_FAILURE);
	}

	cl_logic_generate_placement(cl_placement);
	cl_map = cl_logic_convert_placement(cl_placement);
	cl_gui_input_nick(cl_gui);
	net_send_placement(cl_net, cl_placement);
	cl_main_control(&queue, &cl_map, &cl_enemy_map, cl_net, cl_gui);
	gui_wait(cl_gui);
	net_wait(cl_net);
	return 0;
}

int cl_main_get_options(int argc, char *argv[], int *port, char address[])
{
	char opt, is_address_set = 0, is_port_set = 0;
	while ((opt = getopt(argc, argv, "a:p:")) != -1) {
		switch (opt) {
		case 'a':
			if (!is_address_set) {
				address = optarg;
				is_address_set = 1;
			} else {
				return 1;
			}
			break;
		case 'p':
			if (!is_port_set) {
				*port = atoi(optarg);
				is_port_set = 1;
			} else
				return 1;
			break;
		default:
			return 1;
		}
	}
	if (!(is_address_set && is_port_set))
		return 1;
	return 0;
}

void cl_main_make_event(struct main_queue *queue, enum main_event_types type,
                        void *data, int data_length)
{
	struct main_event *tmp = queue->head->next;

	pthread_mutex_lock(&(queue->mutex));

	for(; tmp->next != NULL; tmp = tmp->next);

	tmp->next = malloc(sizeof(struct main_event));
	tmp = tmp->next;
	tmp->next = NULL;
	tmp->data_length = data_length;
	tmp->event_type = type;
	tmp->event_data = malloc(data_length);
	memcpy(tmp->event_data, data, data_length);
	pthread_mutex_unlock(&(queue->mutex));
	pthread_kill(queue->main_id, SIGUSR1);
	return;
}

void cl_main_control(struct main_queue *queue, map *my_map, map *enemy_map,
                     struct net *network, struct gui *cl_gui)
{
	struct main_event *tmp;
	enum player_state turn;
	sigset_t set;

	/*
	 * Evil while :)
	 */

	while(666) { 
		tmp = cl_main_copy_queue(queue);
		sigemptyset(&set);
		sigaddset(&set, SIGUSR1);

		if(tmp == NULL)
			sigwait(&set, NULL);

		for(; tmp != NULL; tmp = tmp->next) {
			switch (tmp->event_type) {
			case GUI_NICK:
				net_send_nick(network, tmp->event_data, tmp->data_length);
				break;
			case GUI_SHOT:
				cl_main_check_shot(tmp->event_data, my_map, network, cl_gui, &turn);
				break;
			case GUI_EXIT:
				net_stop();
				goto out;
				break;
			case NET_SHOT_RESULT:
				cl_main_check_net_shot(tmp->event_data,
				                       &turn,
				                       my_map,
				                       enemy_map,
				                       cl_gui);
				break;
			case NET_PLACEMENT:
				cl_main_send_placement(tmp->event_data, enemy_map, cl_gui);
				break;
			case NET_ERROR:
				gui_error(cl_gui);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				break;
			case NET_START_GAME:
				cl_main_start_game(tmp->event_data, my_map, &turn, cl_gui);
				break;
			}
		}
		cl_main_clear_queue(tmp);
	}
out:
	cl_main_clear_queue(tmp);
	return;
}

void cl_main_check_shot(void * event_data, map *my_map, struct net *network,
                        struct gui *cl_gui, enum player_state *turn)
{
	char *x = (char *)(event_data);
	char *y = (char *)(event_data + sizeof(char));

	if(logic_valid_shot(*x, *y, my_map, turn) == 0)
		gui_error(cl_gui); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	else
		net_send_shot(network, *x, *y);
}

void cl_main_check_net_shot(void * event_data,
                            enum player_state *turn,
                            map *my_map,
                            map *enemy_map,
                            struct gui *cl_gui)
{
	char *x = (char *)(event_data + sizeof(char));
	char *y = (char *)(event_data + sizeof(char) * 2);
	char *result = (char *)(event_data + sizeof(char) * 3);

	if(turn == WAITING_TURN) {
		enemy_map = logic_shot(x, y, result, enemy_map, turn);
		gui_refresh_map(cl_gui, enemy_map, ENEMY);
	} else {
		my_map = logic_shot(x, y, result, my_map, turn);
		gui_refresh_map(cl_gui, my_map, ME);
	}
}

void cl_main_send_placement(void *event_data, placement *enemy_map,
                            struct gui *cl_gui)
{
	placement *place = (placement *)event_data;
	enemy_map = logic_merge_placement_map(place);
	gui_refresh_map(cl_gui, enemy_map, ENEMY); // send enemy_map to gui. 0 is enemy
}

void cl_main_start_game(void * event_data, map *my_map, enum player_state *turn,
                        struct gui *cl_gui)
{
	turn = (enum player_state *) event_data;
	gui_main_window(cl_gui, my_map);
}

struct main_event *cl_main_copy_queue(struct main_queue *queue)
{
	struct main_event *tmp_head;
	pthread_mutex_lock(&(queue->mutex));
	tmp_head = queue->head->next;
	queue->head->next = NULL;
	pthread_mutex_unlock(&(queue->mutex));
	return tmp_head;
}

void cl_main_clear_queue(struct main_event *head)
{
	struct main_event *prev = head, *tmp = head;
	while(tmp != NULL) {
		free(tmp->event_data);
		tmp = tmp->next;
		free(prev);
		prev = tmp;
	}
}
