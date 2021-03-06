/* vim: set noexpandtab tabstop=4 shiftwidth=4 smartindent: */

#include "cl_main.h"
#include "cl_net.h"

extern char *optarg;

/** Starting point of client
 * \param argc - number of incoming options
 * \param argv - command line options
 * \return ending code
 */
int main(int argc, char *argv[])
{
	int srv_port; /**< Server port */
	char srv_address[ADDRESS_LENGTH]; /**< Server address */
	struct main_event cap = { NULL }, /**< Cap for easy queue processing */
	       *head = &cap; /**< Pointer on head of queue */
	struct main_queue queue /**< All information about queue */
	                        = { pthread_self(), PTHREAD_MUTEX_INITIALIZER, 0, 1,
	                            head };
	//fprintf(stderr, "main: %d ", (int)pthread_self());
	struct net *cl_net = NULL; /**< Net information */
	struct gui *cl_gui = NULL; /**< GUI information */
	map cl_map, /**< Player's map */
	    cl_enemy_map; /**< Enemy's map */
	placement cl_placement; /**< Player's placement to server */
	struct sigaction sigact;
	sigact.sa_handler = usr1_handler;
	// int i, j;

	bzero(cl_enemy_map, sizeof(map));

	if (sigaction(SIGUSR1, &sigact, NULL)) {
		perror("sigaction");
		exit(EXIT_FAILURE);
	}

	/* Process all command line options */
	if (cl_main_get_options(argc, argv, &srv_port, srv_address)) {
		printf("Illegal command line options.\n");
		printf("Use \"-a\" for address and \"-p\" for port.\n");
		exit(EXIT_FAILURE);
	}

	/* Start modules */
	if ((cl_net = cl_net_start(srv_address, srv_port, &queue)) == NULL) {
		printf("Server is unreachable.\n");
		exit(EXIT_FAILURE);
	}

	if ((cl_gui = cl_gui_start(&queue)) == NULL) {
		printf("Can't initialize ncurses interface\n");
		exit(EXIT_FAILURE);
	}
	queue.net_working = 1;
	cl_logic_generate_placement(cl_placement);
	// cl_map = cl_logic_convert_placement(cl_placement);
	memcpy(cl_map, cl_placement, sizeof(map));
	/* for(i = 0; i < 10; ++i) {
			for (j = 0; j < 10; ++j)
				//fprintf(stderr, "%d ", cl_enemy_map[i][j]);
			//fprintf(stderr, "\n");
		} */
	cl_gui_input_nick(cl_gui);
	cl_net_send_placement(cl_net, cl_placement);

	/* Start to process all events */
	cl_main_control(&queue, cl_map, cl_enemy_map, cl_net, cl_gui);

	/* After main work wait for all threads to end */
	cl_gui_wait(cl_gui);
	//cl_net_wait(cl_net);
	if(queue.gui_working)
		cl_gui_stop(cl_gui);
	return 0;
}

/** Command line options processor
 * \param argc - number of incoming options
 * \param argv - command line options
 * \param port - server's port
 * \param address - server's ip address
 * \return 0 if options are correct
 */

int cl_main_get_options(int argc, char *argv[], int *port, char address[])
{
	char opt, /**< letter of option */
	     is_address_set = 0, /**< flag for address */
	     is_port_set = 0; /**< flag for port */
	while ((opt = getopt(argc, argv, "a:p:")) != -1) {
		switch (opt) {
		case 'a': /* Address */
			if (!is_address_set) {
				memcpy(address, optarg, strlen(optarg));
				address[strlen(optarg)] = '\0';
				is_address_set = 1;
			} else {
				return 1;
			}
			break;
		case 'p': /* Port */
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

/** Create event for main queue
 * \param queue - queue parametres
 * \param type - event type
 * \param data - pointer to datafield
 * \param data_length - number of bytes in data
 */

void cl_main_make_event(struct main_queue *queue, enum main_event_types type,
                        void *data, int data_length)
{
	struct main_event *tmp = queue->head; /**< Working pointer */

	pthread_mutex_lock(&(queue->mutex));

	/* Going in the end of list */
	for(; tmp->next != NULL; tmp = tmp->next);

	tmp->next = malloc(sizeof(struct main_event));
	tmp = tmp->next;
	tmp->next = NULL;
	tmp->data_length = data_length;
	tmp->event_type = type;
	if(data_length == 0)
		tmp->event_data = NULL;
	else {
		tmp->event_data = malloc(data_length);
		memcpy(tmp->event_data, data, data_length);
	}
	pthread_mutex_unlock(&(queue->mutex));
	//fprintf(stderr, "event: %d", (int)queue->main_id);
	pthread_kill(queue->main_id, SIGUSR1);
}

/** Processing main queue
 * \param queue - queue parametres
 * \param my_map - curent client map
 * \param enemy_map - enemy map :)
 * \param network - net information
 * \param cl_gui - gui information
 */

void cl_main_control(struct main_queue *queue, map my_map, map enemy_map,
                     struct net *network, struct gui *cl_gui)
{
	struct main_event *tmp; /**< tempory queue pointer*/
	enum player_state turn; /**< whose turn it is to shot */
	sigset_t set; /** */
	int sig;

	/* Evil while :) */
	while(666) { 
		/* copy main queue */
		tmp = cl_main_copy_queue(queue);
		sigemptyset(&set);
		sigaddset(&set, SIGUSR1);
		/* if main queue is emty, wait signal*/
		if(tmp == NULL) {
			sigwait(&set, &sig);
			tmp = cl_main_copy_queue(queue);
		}

		/* for all elements in queue */
		for(; tmp != NULL; tmp = tmp->next) {
			switch (tmp->event_type) {
			case GUI_NICK: /* gui send player nick*/
				cl_net_send_nick(network, tmp->event_data, tmp->data_length);
				cl_gui_main_window(cl_gui, my_map);
				break;
			case GUI_SHOT: /* gui send player's shot*/
				cl_main_check_shot(tmp->event_data, enemy_map, network, cl_gui, &turn);
				break;
			case GUI_EXIT: /* gui send player's whant exit*/
				//fprintf(stderr, "blabla %d", queue->net_working);
				if(queue->net_working)
					cl_net_stop(network);
				cl_gui_stop(cl_gui);
				queue->gui_working = 0;;
				//fprintf(stderr, "we was here!!!\n");
				goto out;
				break;
			case NET_SHOT_RESULT: /* server send shot result*/
				cl_main_check_net_shot(tmp->event_data,
				                       &turn,
				                       my_map,
				                       enemy_map,
				                       cl_gui);
				break;
			case NET_PLACEMENT: /* server send enemy placement*/
				cl_main_send_placement(tmp->event_data, queue, enemy_map,
                                                       cl_gui, turn, network);
				break;
			case NET_ERROR: /* server send some error*/
				//fprintf(stderr, "net_working(NET_ERROR%d)\n", queue->net_working);
				if(queue->net_working) {
					cl_gui_stop(cl_gui);
					cl_net_stop(network);
					queue->gui_working = 0;
					printf("Server close connection\n");
				}
				goto out;
				break;
			case NET_START_GAME: /* server send that the game start*/
				cl_main_start_game(tmp->event_data, my_map, &turn, cl_gui);
				break;
			}
		}
		cl_main_clear_queue(tmp);
	}
out: /* end of evil while*/
	cl_main_clear_queue(tmp);
	return;
}

/** Check player shot
 * \param event_data - shot coordinates
 * \param my_map - curent client map
 * \param network - net information
 * \param cl_gui - gui information
 * \param turn - whose turn to shot
 */

void cl_main_check_shot(void * event_data, map my_map, struct net *network,
                        struct gui *cl_gui, enum player_state *turn)
{
	char x = *(char *)(event_data); /**< x coordinate*/
	char y = *(char *)(event_data + sizeof(char)) /**< y coordinate*/;

	/* if shot not valid */
	//fprintf(stderr, "%d\n", my_map[(int)y][(int)x]);
	if(cl_logic_valid_shot(x, y, my_map, turn) == 0) {
		cl_gui_refresh_status(cl_gui, INVALID_SHOT);
		// cl_gui_error(cl_gui);
	} else {
		cl_net_send_shot(network, x, y);
	}
}

/** Check shot result from server
 * \param event_data - shot coordinates and result
 * \param turn - whose turn to shot
 * \param my_map - player map
 * \param enemy_map - enemy map :)
 * \param cl_gui - gui information
 */

void cl_main_check_net_shot(void * event_data,
                            enum player_state *turn,
                            map my_map,
                            map enemy_map,
                            struct gui *cl_gui)
{
	char x = *(char *)(event_data ); /**< x coordinate*/
	char y = *(char *)(event_data + sizeof(char)); /**< y coordinate*/
	char result = *(char *)(event_data + sizeof(char) * 2); /**< shot result*/
	// int i, j;
	// printf("%d %d %d\n", x, y, result);
	/* if playershoting and whating answer from server*/
	if(*turn == WAITING_TURN) {
		cl_logic_shot(x, y, result, enemy_map, turn);
		cl_gui_refresh_map(cl_gui, enemy_map, ENEMY);
	} else {
		cl_logic_shot(x, y, result, my_map, turn);
		cl_gui_refresh_map(cl_gui, my_map, ME);
	}

	if (*turn == MY_TURN)
		cl_gui_refresh_status(cl_gui, YOU_TURN);
	else
		cl_gui_refresh_status(cl_gui, NOT_YOU_TURN);
}

/** Take enemy placement when game end
 * \param event_data - enemy placement
 * \param enemy_map - enemy map :)
 * \param cl_gui - gui information
 */

void cl_main_send_placement(void *event_data, struct main_queue *queue, map enemy_map,
                            struct gui *cl_gui, enum player_state turn, struct net *cl_net)
{
	placement place;
	memcpy(place, event_data, sizeof(placement));  // FIX ME!!!!!!

	if(queue->net_working) {
		cl_net_stop(cl_net);
		queue->net_working = 0;
	}

	cl_logic_merge_placement_map(place, enemy_map);
	cl_gui_refresh_map(cl_gui, enemy_map, ENEMY);

	if(turn == MY_TURN || turn == WAITING_TURN)
		cl_gui_refresh_status(cl_gui, YOU_WIN);
	else
		cl_gui_refresh_status(cl_gui, YOU_LOSE);

}

/** Start game
 * \param event_data - whose turn to shot
 * \param my_map - curent client map
 * \param turn - whose turn to shot
 * \param cl_gui - gui information
 */

void cl_main_start_game(void * event_data, map my_map, enum player_state *turn,
                        struct gui *cl_gui)
{
	*turn = *((char *)event_data);
	switch(*turn) {
	case SRV_NET_ENEMY_TURN:
		*turn = ENEMY_TURN;
		break;
	case SRV_NET_YOUR_TURN:
		*turn = MY_TURN;
		break;
	default:
		//printf("Wrong turn data\n");
		exit(EXIT_FAILURE);
	}
	if (*turn == MY_TURN)
		cl_gui_refresh_status(cl_gui, YOU_TURN);
	else
		cl_gui_refresh_status(cl_gui, NOT_YOU_TURN);
}

/** Copy main queue
 * \param queue - queue parametres
 * \return a pointer to the queue head
 */

struct main_event *cl_main_copy_queue(struct main_queue *queue)
{
	struct main_event *tmp_head;

	/* lock queue for nothing to lose*/
	pthread_mutex_lock(&(queue->mutex));
	tmp_head = queue->head->next;
	queue->head->next = NULL;

	/* unlock queue*/
	pthread_mutex_unlock(&(queue->mutex));
	return tmp_head;
}

/** Clear memory after work with queue
 * \param head - start of queue
 */

void cl_main_clear_queue(struct main_event *head)
{
	struct main_event *prev = head; /**< pointer to prev element*/
	struct main_event *tmp = head; /**< pointer to curent element*/
	while(tmp != NULL) {
		/* release the memory data*/
		free(tmp->event_data);
		/* go to next element*/
		tmp = tmp->next;
		/* release the memory of one item in thr queue*/
		free(prev);
		/* go to curent element*/
		prev = tmp;
	}
}

void usr1_handler(int a)
{
}
