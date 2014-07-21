/* vim: set noexpandtab tabstop=4 shiftwidth=4 smartindent: */
#include "cl_gui.h"

void gui_key_processing_battleground(struct gui *options, long ch)
{

}


void *gui_key_processing(void *arg)
{
	struct gui *options;
	long ch;

	options = (struct gui*)arg;

	keypad(stdscr, TRUE);

	while((ch = getch()) != KEY_END) {
	/*
	 * options->state = 0 - enter_nickname;
	 * options->state = 1 - work with battleground;
	 * options->state = 2 - work with chat;
	 */
		switch(options->state) {
			case 1:
				gui_key_processing_battleground(options, ch);
				break;

			case 2:
				gui_key_processing_chat(options, ch);
				break;
		}
	}

	keypad(stdscr, FALSE);

	gui_stop(options);

}

struct gui *gui_start(struct main_queue *main_queue_h)
{
	struct gui *options;
	pthread_t gui_pthread;
	int idx, jdx;

	initscr();

	if(getmaxx(stdscr) < 94 || getmaxy(stdscr) < 39) {
		endwin();
		printf("ERROR::Low Resolution: Resolution at least must be 94x39\n");
		return NULL;
	}

	start_color();

	for(idx = 1; idx < 9; idx++) {
		for(jdx = 1; jdx < 9; jdx++) {
			init_pair( ((jdx * 10) + idx), jdx, idx);
		}
	}

	options = (struct gui *) malloc(sizeof(struct gui));

	options->bg_color = 1;
	options->font_color = 8;
	options->state = 0;
	options->main_queue_head = main_queue_h;
	options->size_of_msg = 0;
	options->x = 0;
	options->y = 0;

	for(idx = 0; idx < MAX_BUFF; idx++) {
		options->msg[idx] = '\0';
	}

	pthread_create(&gui_pthread, NULL, gui_key_processing, (void *) options);

	return options;
}

int gui_input_nick(struct gui *options)
{
        char nick[16];

        refresh();
        options->nick_window = newwin(5, 50, 10, 20);
        wbkgd(options->nick_window, COLOR_PAIR(CUR_COLOR));
        box(options->nick_window, ACS_VLINE, ACS_HLINE);

        echo();

        wmove(options->nick_window, 2, 2);
        wprintw(options->nick_window, "Enter your nickname: ");
        curs_set(TRUE);
        wmove(options->nick_window, 2, 23);
        wgetnstr(options->nick_window, nick, 16);
        curs_set(FALSE);

        noecho();

        wrefresh(options->nick_window);
        delwin(options->nick_window);

        clear();
        refresh();

        cl_main_make_event(options->main_queue_head, GUI_NICK, (void*)nick, strlen(nick));

        options->state = 1;
        return 0;
}

int gui_main_window(struct gui *options, char map[10][10])
{
	int idx, jdx;
	int poz = 4;
	refresh();

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
	attrset(COLOR_PAIR(13));
	printw("You");
	attroff(COLOR_PAIR(13));
	refresh();

	wattron(options->my_map, COLOR_PAIR(13));
	for(idx = 0; idx < 10; idx++) {
		for(jdx = 0; jdx < 10; jdx++) {
			if(map[idx][jdx] == 1) {
				wmove(options->my_map, 1 + (idx * 2), 2 + (jdx * 4));
				wprintw(options->my_map,"#");
			}
		}
	}
	wattroff(options->my_map, COLOR_PAIR(13));
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

	options->chat = newwin((getmaxy(stdscr) - 27), 89, 26, 4);
	wbkgd(options->chat, COLOR_PAIR(CUR_COLOR));
	box(options->chat, ACS_VLINE, ACS_HLINE);
	wrefresh(options->chat);

	options->line_stat = newwin(1, 89, getmaxy(stdscr) - 1, 4);
	wbkgd(options->line_stat, COLOR_PAIR(18));
	wmove(options->line_stat, 0, 0);
	wprintw(options->line_stat, "Status: ");
	wrefresh(options->line_stat);

	return 0;
}
