#ifndef CL_LOGIC__H__
#define CL_LOGIC__H__

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"
#include "../include/net_types.h"

void  cl_logic_generate_placement(placement map);
void  cl_logic_merge_placement_map(placement server_map, map cl_map);
void  cl_logic_shot(int x, int y, enum srv_net_shot_result *result, map cl_map, enum player_state *state);
int   cl_logic_kill_ship(int x, int y, int result, map  cl_map);
int   cl_logic_valid_shot(int x, int y, map cl_map, enum player_state *state);
char* cl_logic_convert_placement(placement fild1);

#endif
