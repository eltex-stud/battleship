/* vim: set noexpandtab tabstop=4 shiftwidth=4 smartindent: */
#ifndef CL_LOGIC__H__
#define CL_LOGIC__H__

#include <time.h>
#include <stdlib.h>
#include "client.h"

void cl_logic_generate_placement(placement map[10][10]);
void cl_logic_merge_placement_map(placement server_map[10][10], placement map[10][10]);
void cl_logic_shot(int x, int y, int result, placement map[10][10], enum player_state *state);
void cl_logic_kill_ship(int x, int y, int result, placement map[10][10]);

/*#include "cl_logic_generate_placement.c"
#include "cl_logic_shot.c"
#include "cl_logic_marge_placement_map.c"
#include "cl_logic_valid_shot.c"
#include "cl_logic_kill_ship.c"*/

#endif
