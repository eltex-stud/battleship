/* vim: set noexpandtab tabstop=4 shiftwidth=4 smartindent: */
#ifndef BATTLESHIP_CLIENT_GUI_H_
#define BATTLESHIP_CLIENT_GUI_H_

#include "cl_main.c"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <curses.h>
#include <pthread.h>


#define CUR_COLOR ((bg_color * 10) + font_color)

void *cl_gui_key_processing(void *arg);

struct gui *gui_start(struct main_queue *main_queue_h);

int gui_input_nick(struct gui *options);

int gui_main_window(struct gui *options, char map[10][10]);

struct gui
{
	WINDOW *nick_window;
	WINDOW *my_map;
	WINDOW *enemy_map;
	WINDOW *chat;
	WINDOW *line_stat;
	unsigned short bg_color;
	unsigned short font_color;
	unsigned short state;
	struct main_queue *main_queue_head;
};

#endif // BATTLESHIP_CLIENT_GUI_H_
