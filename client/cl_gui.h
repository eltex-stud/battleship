/* vim: set noexpandtab tabstop=4 shiftwidth=4 smartindent: */
#ifndef BATTLESHIP_CLIENT_GUI_H_
#define BATTLESHIP_CLIENT_GUI_H_

#include "client.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <curses.h>
#include <pthread.h>
#include <signal.h>

#define MAX_BUFF 86

#define HEIGHT 33
#define WIDTH 94

#define ENTER 10
#define KEY_SPACE 32

#define CUR_COLOR ((options->bg_color * 10) + options->font_color)

#define CLR_BLK_WHT 18
#define CLR_BLK_GRN 13
#define CLR_BLK_YLLW 14
#define CLR_BLK_RED 12
#define CLR_GRN_WHT 38
#define CLR_RED_WHT 28

enum gui_status_line { YOU_TURN, NOT_YOU_TURN, INVALID_SHOT};

enum gui_state { STATE_NICK, STATE_YOU_TURN, STATE_ENEMY_TURN, STATE_CHAT, 
				 STATE_WAIT};

struct gui
{
	WINDOW *nick_window;
	WINDOW *my_map;
	WINDOW *enemy_map;
	WINDOW *chat;
	WINDOW *line_stat;
	unsigned short bg_color;
	unsigned short font_color;
	unsigned short size_of_msg;
	unsigned short x;
	unsigned short y;
	char msg[MAX_BUFF];
	struct main_queue *main_queue_head;
	enum gui_state state;
	pthread_t gui_id;
	pthread_mutex_t mutex;
};

void gui_distinguish_cell(struct gui *options, unsigned short color);

void *gui_key_processing(void *arg);

void gui_key_processing_battleground(struct gui *options, long ch);

void gui_key_processing_chat(struct gui *options, long ch);

void gui_key_processing_input_nick(struct gui *options, long ch);

struct gui *cl_gui_start(struct main_queue *main_queue_h);

void cl_gui_stop(struct gui *options);

void cl_gui_wait(struct gui *options);

int cl_gui_input_nick(struct gui *options);

void gui_my_window(struct gui *options, map cl_map);

void gui_enemy_window(struct gui *options);

void gui_chat(struct gui *options);

void gui_status_line(struct gui *options);

int cl_gui_main_window(struct gui *options, map cl_map);

void cl_gui_refresh_status(struct gui *options, enum gui_status_line turn);

int cl_gui_refresh_map(struct gui *options, map came_map, enum player pl);

void gui_usr1_handler(int a);

#endif // BATTLESHIP_CLIENT_GUI_H_
