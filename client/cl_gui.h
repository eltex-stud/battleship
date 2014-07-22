/* vim: set noexpandtab tabstop=4 shiftwidth=4 smartindent: */
#ifndef BATTLESHIP_CLIENT_GUI_H_
#define BATTLESHIP_CLIENT_GUI_H_

#include "cl_main.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <curses.h>
#include <pthread.h>

#define MAX_BUFF 128

#define KEY_ENTER 10

#define CUR_COLOR ((options->bg_color * 10) + options->font_color)

int gui_refresh_map(struct gui *options, char map[10][10], enum player pl);

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
	unsigned short size_of_msg;
	unsigned short x;
	unsigned short y;
	char msg[MAX_BUFF];
	struct main_queue *main_queue_head;
};

void *gui_key_processing(void *arg);

void gui_key_processing_battleground(struct gui *options, long ch);

void gui_key_processing_chat(struct gui *options, long ch);

struct gui *gui_start(struct main_queue *main_queue_h);

int gui_input_nick(struct gui *options);

int gui_main_window(struct gui *options, char map[10][10]);

void gui_stop(struct gui *options);

#endif // BATTLESHIP_CLIENT_GUI_H_
