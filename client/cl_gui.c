/* vim: set noexpandtab tabstop=4 shiftwidth=4 smartindent: */
#include "cl_gui.h"

void gui_distinguish_cell(struct gui *options, unsigned short bgcolor)
{
/*	chtype symb;
	int idx;
	int jdx;
*/

/*	wclear(options->line_stat);
	wmove(options->line_stat, 0, 0);
	wprintw(options->line_stat, "%c%d", 'A' + options->x, 1 + options->y);
	wrefresh(options->line_stat);

	for (idx = 0; idx < 2; idx++) {
		for(jdx = 0; jdx < 3; jdx++) {
			wmove(options->enemy_map, 1 + idx + (options->y * 2), 1 + jdx + (options->x * 4));
			if (idx != 0 && jdx != 1) {

			}
*/
	wmove(options->enemy_map, 1 + (options->y * 2), 2 + (options->x * 4));
	wrefresh(options->enemy_map);
}

void gui_key_processing_input_nick(struct gui *options, long ch)
{
	int idx;

	pthread_mutex_lock(&(options->mutex));
	switch(ch) {
		case ENTER:
			cl_main_make_event(options->main_queue_head, GUI_NICK, (void*)options->msg, options->size_of_msg);

			for(idx = 0; idx < MAX_BUFF; idx++) {
				options->msg[idx] = '\0';
			}
			options->size_of_msg = 0;

			wclear(options->nick_window);
			delwin(options->nick_window);
			clear();
			refresh();

			 options->state =  STATE_WAIT;
			break;

		case KEY_BACKSPACE:
			if(options->size_of_msg > 0) {
				options->size_of_msg--;
				wmove(options->nick_window, 2, 23 + options->size_of_msg);
				wprintw(options->nick_window, " ");
				wmove(options->nick_window, 2, 23 + options->size_of_msg);
				options->msg[options->size_of_msg] = '\0';
				wrefresh(options->nick_window);
			}
			break;

		default:
			if (options->size_of_msg < 26 - 1) {
				options->msg[options->size_of_msg] = ch;
				wmove(options->nick_window, 2, 23 + options->size_of_msg);
				wprintw(options->nick_window, "%c", ch);
				options->size_of_msg++;
				wrefresh(options->nick_window);
			}
			break;
	}
	pthread_mutex_unlock(&(options->mutex));
}

void gui_key_processing_chat(struct gui *options, long ch)
{
	pthread_mutex_lock(&(options->mutex));
	switch(ch) {
		case ENTER:
			options->state = STATE_YOU_TURN;
			break;

		default:
			break;
	}
	pthread_mutex_unlock(&(options->mutex));
}

void gui_key_processing_battleground(struct gui *options, long ch)
{
	char data[2];

	pthread_mutex_lock(&(options->mutex));
	switch(ch) {
		case ENTER:
			options->state = STATE_CHAT;
			wmove(options->chat, getmaxy(options->chat) - 1, 1);
//			curs_set(TRUE);
			wrefresh(options->chat);
			break;
		
		case KEY_UP:

			if(options->y == 0) {
				options->y = 9;
			} else {
				options->y--;
			}
			gui_distinguish_cell(options, CUR_COLOR);
			break;

		case KEY_DOWN:
			if(options->y == 9) {
				options->y = 0;
			} else {
				options->y++;
			}
			gui_distinguish_cell(options, CUR_COLOR);
			break;

		case KEY_LEFT:
			if(options->x == 0) {
				options->x = 9;
			} else {
				options->x--;
			}
			gui_distinguish_cell(options, CUR_COLOR);
			break;

		case KEY_RIGHT:
			if(options->x == 9) {
				options->x = 0;
			} else {
				options->x++;
			}
			gui_distinguish_cell(options, CUR_COLOR);
			break;

		case KEY_SPACE:
			if (options->state == STATE_YOU_TURN) {
				data[0] = options->x;
				data[1] = options->y;
				cl_main_make_event(options->main_queue_head, GUI_SHOT, (void *)data, 2);
			}
			break;
	}
	pthread_mutex_unlock(&(options->mutex));

}


void *gui_key_processing(void *arg)
{
	struct gui *options;
	long ch;
	struct sigaction sigact;
	sigact.sa_handler = gui_usr1_handler;

	if (sigaction(SIGUSR1, &sigact, NULL)) {
		perror("sigaction");
		exit(EXIT_FAILURE);
	}

	options = (struct gui*)arg;

	keypad(stdscr, TRUE);

	while((ch = getch()) != KEY_END) {
		switch(options->state) {
			case STATE_NICK:
				gui_key_processing_input_nick(options, ch);
				break;

			case STATE_YOU_TURN:
				gui_key_processing_battleground(options, ch);
				break;

			case STATE_ENEMY_TURN:
				gui_key_processing_battleground(options, ch);
				break;

			case STATE_CHAT:
				gui_key_processing_chat(options, ch);
				break;

			case STATE_WAIT:
				break;
		}
	}

	keypad(stdscr, FALSE);
	cl_main_make_event(options->main_queue_head, GUI_EXIT, NULL, -1);

	return NULL;
}

struct gui *cl_gui_start(struct main_queue *main_queue_h)
{
	struct gui *options;
	int idx;
	int jdx;

	initscr();
	cbreak();
	noecho();

	if(getmaxx(stdscr) < WIDTH || getmaxy(stdscr) < HEIGHT) {
		endwin();
		printf("ERROR::Low Resolution: Resolution at least must be %dx%d (HxW)\n", WIDTH, HEIGHT);
		return NULL;
	}

	start_color();

	for(idx = 1; idx < 9; idx++) {
		for(jdx = 1; jdx < 9; jdx++) {
			init_pair( ((idx * 10) + jdx), jdx - 1, idx - 1);
		}
	}

	options = (struct gui *) malloc(sizeof(struct gui));

	options->bg_color = 1;
	options->font_color = 8;
	options->state = STATE_WAIT;
	options->main_queue_head = main_queue_h;
	options->size_of_msg = 0;
	options->x = 0;
	options->y = 0;

	bkgdset(COLOR_PAIR(CUR_COLOR));
	clear();
	refresh();

	pthread_mutex_init (&(options->mutex), NULL);

	for(idx = 0; idx < MAX_BUFF; idx++) {
		options->msg[idx] = '\0';
	}

	pthread_create(&(options->gui_id), NULL, gui_key_processing, (void *) options);

	return options;
}

void cl_gui_wait(struct gui *options)
{
	pthread_join(options->gui_id, NULL);
}

void cl_gui_stop(struct gui *options)
{
	delwin(options->my_map);
	delwin(options->enemy_map);
	delwin(options->chat);
	delwin(options->line_stat);

	endwin();

	pthread_mutex_destroy (&options->mutex);

	free(options);
}


int cl_gui_input_nick(struct gui *options)
{
	pthread_mutex_lock(&(options->mutex));

	refresh();
	options->nick_window = newwin(5, 50, 10, 20);
	wbkgd(options->nick_window, COLOR_PAIR(CUR_COLOR));
	box(options->nick_window, ACS_VLINE, ACS_HLINE);

	wmove(options->nick_window, 2, 2);
	wprintw(options->nick_window, "Enter your nickname: ");
	curs_set(TRUE);
	wmove(options->nick_window, 2, 23);
	wrefresh(options->nick_window);
	options->state = STATE_NICK;

	pthread_mutex_unlock(&(options->mutex));

	return 0;
}

int cl_gui_main_window(struct gui *options, map cl_map)
{
	int idx, jdx;
	int poz = 4;

	pthread_mutex_lock(&(options->mutex));
	refresh();

	move(0, 5);
	
	attron(A_BOLD);
	
	printw("Move: ");

	attron(A_ALTCHARSET);
	printw("%c%c%c%c%c%c%c", ACS_UARROW, ACS_VLINE, ACS_DARROW, ACS_VLINE, 
							 ACS_LARROW, ACS_VLINE, ACS_RARROW);
	attroff(A_ALTCHARSET);
	printw("; Shot: space; Enter chat/battlefield: ENTER;");
	
	move(1, 5);
	attron(COLOR_PAIR(CLR_BLK_GRN));
	printw("#");
	attroff(COLOR_PAIR(CLR_BLK_GRN));
	printw(" - ship;");

	attron(COLOR_PAIR(CLR_BLK_YLLW));
	printw("X");
	attroff(COLOR_PAIR(CLR_BLK_YLLW));
	printw(" - wounded;");

	attron(COLOR_PAIR(CLR_BLK_RED));
	printw("X");
	attroff(COLOR_PAIR(CLR_BLK_RED));
	printw(" - dead;");

	attron(COLOR_PAIR(CLR_BLK_WHT));
	printw("*");
	attroff(COLOR_PAIR(CLR_BLK_WHT));
	printw(" - miss;");

	attroff(A_BOLD);

	for(idx = 0; idx < 10; idx++) {
		move(poz, 2);
		poz = poz + 2;
		printw("%d\n", idx+1);
		refresh();
	}

	poz = 5;
	for(idx = 0; idx < 10; idx++) {
		move(2, poz);
		poz += 4;
		printw("%c", 'A' + idx);
	}
	refresh();

	options->my_map = newwin(22, 41, 3, 4);
	wbkgd(options->my_map, COLOR_PAIR(CUR_COLOR));

	for(idx = 2; idx < 20; idx += 2) {
		for(jdx = 1; jdx < 41; jdx++) {
			wmove(options->my_map, idx, jdx);
			wprintw(options->my_map, "_");
			wrefresh(options->my_map);
		}
	}

	for(idx = 4; idx < 40; idx += 4) {
		for(jdx = 1; jdx < 21; jdx++) {
			wmove(options->my_map, jdx, idx);
			wprintw(options->my_map, "|");
			wrefresh(options->my_map);
		}
	}

	box(options->my_map, ACS_VLINE, ACS_HLINE);

	move(25, 4);
	attrset(COLOR_PAIR(CLR_BLK_GRN));
	printw("You");
	attroff(COLOR_PAIR(CLR_BLK_GRN));
	refresh();

	wattron(options->my_map, COLOR_PAIR(CLR_BLK_GRN));
	for(idx = 0; idx < 10; idx++) {
		for(jdx = 0; jdx < 10; jdx++) {
			if(cl_map[idx][jdx] == MAP_SHIP) {
				wmove(options->my_map, 1 + (idx * 2), 2 + (jdx * 4));
				wprintw(options->my_map,"#");
			}
		}
	}
	wattroff(options->my_map, COLOR_PAIR(CLR_BLK_GRN));
	wrefresh(options->my_map);


	options->enemy_map = newwin(22, 41, 3, 52);
	wbkgd(options->enemy_map, COLOR_PAIR(CUR_COLOR));

	poz = 4;
	for(idx = 0; idx < 10; idx++) {
		move(poz, 50);
		poz = poz + 2;
		printw("%d\n", idx+1);
		refresh();
	}

	poz = 53;
	for(idx = 0; idx < 10; idx++) {
		move(2, poz);
		poz += 4;
		printw("%c", 'A' + idx);
	}

	for(idx = 2; idx < 20; idx += 2) {
		for(jdx = 1; jdx < 41; jdx++) {
			wmove(options->enemy_map, idx, jdx);
			wprintw(options->enemy_map, "_");
			wrefresh(options->enemy_map);
		}
	}

	for(idx = 4; idx < 40; idx += 4) {
		for(jdx = 1; jdx < 21; jdx++) {
			wmove(options->enemy_map, jdx, idx);
			wprintw(options->enemy_map, "|");
			wrefresh(options->enemy_map);
		}
	}

	box(options->enemy_map, ACS_VLINE, ACS_HLINE);
	wrefresh(options->enemy_map);

	move(25, 53);
	attrset(COLOR_PAIR(2));
	printw("Enemy: hair monster");
	attroff(COLOR_PAIR(2));
	refresh();

	options->chat = newwin(getmaxy(stdscr) - 27, 89, 26, 4);
	wbkgd(options->chat, COLOR_PAIR(CUR_COLOR));
	box(options->chat, ACS_VLINE, ACS_HLINE);
	wrefresh(options->chat);

	options->line_stat = newwin(1, 89, getmaxy(stdscr) - 1, 4);
	wbkgd(options->line_stat, COLOR_PAIR(CUR_COLOR));
	wattron(options->line_stat, A_BOLD);
	wattron(options->line_stat, COLOR_PAIR(CLR_RED_WHT));
	
	wclear(options->line_stat);

	wmove(options->line_stat, 0, 1);
	wprintw(options->line_stat, "STATUS: WAIT YOUR ENEMY");
	
	wattroff(options->line_stat, COLOR_PAIR(CLR_RED_WHT));
	wrefresh(options->line_stat);

	pthread_mutex_unlock(&(options->mutex));

	return 0;
}

void cl_gui_refresh_status(struct gui *options, enum gui_status_line turn)
{
	int idx;

	pthread_mutex_lock(&(options->mutex));
	switch(turn) {
		case YOU_TURN:
			wattron(options->line_stat, COLOR_PAIR(CLR_GRN_WHT));
			wmove(options->line_stat, 0, 0);

			for(idx = 0; idx < getmaxx(options->line_stat); idx++) {
				wprintw(options->line_stat, " ");
			}

			wmove(options->line_stat, 0, 1);

			wprintw(options->line_stat, "STATUS: YOUR TURN");
			wattroff(options->line_stat, COLOR_PAIR(CLR_GRN_WHT));
			wrefresh(options->line_stat);

			options->state = STATE_YOU_TURN;
			break;

		case NOT_YOU_TURN:
			wattron(options->line_stat, COLOR_PAIR(CLR_RED_WHT));
			wmove(options->line_stat, 0, 0);
			
			for(idx = 0; idx < getmaxx(options->line_stat); idx++) {
				wprintw(options->line_stat, " ");
			}

			wmove(options->line_stat, 0, 1);

			wprintw(options->line_stat, "STATUS: ENEMY_TURN");
			wattroff(options->line_stat, COLOR_PAIR(CLR_RED_WHT));
			wrefresh(options->line_stat);

			options->state = STATE_ENEMY_TURN;
			break;
	}
	pthread_mutex_unlock(&(options->mutex));
}

int cl_gui_refresh_map(struct gui *options, map came_map, enum player pl)
{
	int idx;
	int jdx;
	pthread_mutex_lock(&(options->mutex));
	switch(pl) {
		case ME:
			for(idx = 0; idx < 10; idx++) {
				for(jdx = 0; jdx < 10; jdx++) {
					if(came_map[idx][jdx] == MAP_SHIP) {
						wmove(options->my_map, 1 + (idx * 2), 2 + (jdx * 4));
						wattron(options->my_map, COLOR_PAIR(CLR_BLK_GRN));
						wprintw(options->my_map, "#");
						wattroff(options->my_map, COLOR_PAIR(CLR_BLK_GRN));
					}
					if(came_map[idx][jdx] == MAP_WOUNDED) {
						wmove(options->my_map, 1 + (idx * 2), 2 + (jdx * 4));
						wattron(options->my_map, A_BOLD);
						wattron(options->my_map, COLOR_PAIR(CLR_BLK_YLLW));
						wprintw(options->my_map, "X");
						wattroff(options->my_map, COLOR_PAIR(CLR_BLK_YLLW));
						wattroff(options->my_map, A_BOLD);
					}
					if(came_map[idx][jdx] == MAP_KILLED) {
						wmove(options->my_map, 1 + (idx * 2), 2 + (jdx * 4));
						wattron(options->my_map, A_BOLD);
						wattron(options->my_map, COLOR_PAIR(CLR_BLK_RED));
						wprintw(options->my_map, "X");
						wattroff(options->my_map, COLOR_PAIR(CLR_BLK_RED));
						wattroff(options->my_map, A_BOLD);
					}
					if(came_map[idx][jdx] == MAP_MISS) {
						wmove(options->my_map, 1 + (idx * 2), 2 + (jdx * 4));
						wattron(options->my_map, COLOR_PAIR(CUR_COLOR));
						wattron(options->my_map, A_BOLD);
						wprintw(options->my_map, "*");
						wattroff(options->my_map, COLOR_PAIR(CUR_COLOR));
						wattroff(options->my_map, A_BOLD);
					}
				}
			}
			wrefresh(options->my_map);
			break;
		case ENEMY:
			for(idx = 0; idx < 10; idx++) {
				for(jdx = 0; jdx < 10; jdx++) {
					if(came_map[idx][jdx] == MAP_WOUNDED) {
						wmove(options->enemy_map, 1 + (idx * 2), 2 + (jdx * 4));
						wattron(options->enemy_map, A_BOLD);
						wattron(options->enemy_map, COLOR_PAIR(CLR_BLK_YLLW));
						wprintw(options->enemy_map, "X");
						wattroff(options->enemy_map, COLOR_PAIR(CLR_BLK_YLLW));
						wattroff(options->enemy_map, A_BOLD);
					}
					if(came_map[idx][jdx] == MAP_KILLED) {
						wmove(options->enemy_map, 1 + (idx * 2), 2 + (jdx * 4));
						wattron(options->enemy_map, A_BOLD);
						wattron(options->enemy_map, COLOR_PAIR(CLR_BLK_RED));
						wprintw(options->enemy_map, "X");
						wattroff(options->enemy_map, COLOR_PAIR(CLR_BLK_RED));
						wattroff(options->enemy_map, A_BOLD);
					}
					if(came_map[idx][jdx] == MAP_MISS) {
						wmove(options->enemy_map, 1 + (idx * 2), 2 + (jdx * 4));
						wattron(options->enemy_map, COLOR_PAIR(CUR_COLOR));
						wattron(options->enemy_map, A_BOLD);
						wprintw(options->enemy_map, "*");
						wattroff(options->enemy_map, COLOR_PAIR(CUR_COLOR));
						wattroff(options->enemy_map, A_BOLD);
					}
				}
			}
			wrefresh(options->enemy_map);
			break;
	}
	pthread_mutex_unlock(&(options->mutex));
	return 0;
}

void gui_usr1_handler(int a)
{
}
