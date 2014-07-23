#ifndef CL_LOGIC__H__
#define CL_LOGIC__H__

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"

void  cl_logic_generate_placement(placement map);
void  cl_logic_merge_placement_map(placement server_map, map cl_map);
void  cl_logic_shot(int x, int y, char result, placement map, enum player_state *state);
void  cl_logic_kill_ship(int x, int y, char result, placement map);
int   cl_logic_valid_shot(int x, int y, placement map, enum player_state *state);
char* cl_logic_convert_placement(placement fild1);

#endif
