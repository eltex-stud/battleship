/* vim: set noexpandtab tabstop=4 shiftwidth=4 smartindent: */
#include "cl_gui.h"


void *cl_gui_key_processing(void *arg)
{
	struct gui *options;

	options = (struct gui*)arg;

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

	pthread_create(&gui_pthread, NULL, cl_gui_key_processing, (void *) options);

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

