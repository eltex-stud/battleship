/* vim: set noexpandtab tabstop=4 shiftwidth=4 smartindent: */
#include "cl_logic.h"

void cl_logic_generate_placement(placement map)
{
    int index1, index2, n, m, location;
    
    for(index1 = 0; index1 < 10; index1++) {
	for(index2 = 0; index2 < 10; index2++) {
	    map[index1][index2] = 0;
	}
    }
    
    srand(time(NULL));
    
    /*
     * the four-ship
     */
    location = rand() % 2;
    
    switch(location) {
	case 0: { /* vertical location */
	    m = rand() % 7;
	    n = rand() % 10;
	    
	    while((map[m][n] == 1) || (map[m + 1][n] == 1) || (map[m + 2][n] == 1) ||
	          (map[m + 3][n] == 1) || (map[m - 1][n - 1] == 1) || (map[m - 1][n] == 1) ||
	          (map[m - 1][n + 1] == 1) || (map[m][n - 1] == 1) || (map[m][n + 1] == 1) ||
	          (map[m + 1][n - 1] == 1) || (map[m + 1][n + 1] == 1) || (map[m + 2][n - 1] == 1) ||
	          (map[m + 2][n + 1] == 1) || (map[m + 3][n - 1] == 1) || (map[m + 3][n + 1] == 1) ||
	          (map[m + 4][n - 1] == 1) || (map[m + 4][n] == 1) || (map[m + 4][n + 1] == 1)) {
		m = rand() % 7;
		n = rand() % 10;
	    }
	    
	    map[m][n] = 1;
	    map[m + 1][n] = 1;
	    map[m + 2][n] = 1;
	    map[m + 3][n] = 1;
	    
	    break;
	}
	
	case 1: { /* horizontal location */
	    m = rand() % 10;
	    n = rand() % 7;
	    
	    while(map[m][n] == 1 || map[m][n + 1] == 1 || map[m][n + 2] == 1 || map[m][n + 3] == 1 ||
	          map[m - 1][n - 1] == 1 || map[m - 1][n] == 1 || map[m - 1][n + 1] == 1 || map[m - 1][n + 2] == 1 ||
	          map[m - 1][n + 3] == 1 || map[m - 1][n + 4] == 1 || map[m][n - 1] == 1 || map[m][n + 4] == 1 ||
	          map[m + 1][n - 1] == 1 || map[m + 1][n] == 1 || map[m + 1][n + 1] == 1 || map[m + 1][n + 2] == 1 ||
	          map[m + 1][n + 3] == 1 || map[m + 1][n + 4] == 1) {
		m = rand() % 10;
		n = rand() % 7;
	    }
	    
	    map[m][n] = 1;
	    map[m][n + 1] = 1;
	    map[m][n + 2] = 1;
	    map[m][n + 3] = 1;
	    
	    break;
	}
    }
    
    /*
     * three-deck ship
     */
    for(index1 = 0; index1 < 2; index1++) {
	location = rand() % 2;
	
	switch(location) {
	    case 0: /* vertical location */
	    {
		m = rand() % 8;
		n = rand() % 10;
		
		while(map[m][n] == 1 || map[m + 1][n] == 1 || map[m + 2][n] == 1 ||
		      map[m - 1][n - 1] == 1 || map[m - 1][n] == 1 || map[m - 1][n + 1] == 1 ||
		      map[m][n - 1] == 1 || map[m][n + 1] == 1 || map[m + 1][n - 1] == 1 ||
		      map[m + 1][n + 1] == 1 || map[m + 2][n - 1] == 1 || map[m + 2][n + 1] == 1 ||
		      map[m + 3][n - 1] == 1 || map[m + 3][n] == 1 || map[m + 3][n + 1] == 1) {
		    m = rand() % 8;
		    n = rand() % 10;
		}
		
		map[m][n] = 1;
		map[m + 1][n] = 1;
		map[m + 2][n] = 1;
		
		break;
	    }
	    
	    case 1: { /* horizontal location */
		m = rand() % 10;
		n = rand() % 8;
		
		while(map[m][n] == 1 || map[m][n + 1] == 1 || map[m][n + 2] == 1 ||
		      map[m - 1][n - 1] == 1 || map[m - 1][n] == 1 || map[m - 1][n + 1] == 1 ||
		      map[m - 1][m + 2] == 1 || map[m - 1][n + 3] == 1 || map[m][n - 1] == 1 ||
		      map[m][n + 3] == 1 || map[m + 1][n - 1] == 1 || map[m + 1][n] == 1 ||
		      map[m + 1][n + 1] == 1 || map[m + 1][n + 2] == 1 || map[m + 1][n + 3] == 1) {
		    m = rand() % 10;
		    n = rand() % 8;
		}
		
		map[m][n] = 1;
		map[m][n + 1] = 1;
		map[m][n + 2] = 1;
		
		break;
	    }
	}
    }
    
    /*
     * double-decked ship
     */
    for(index1 = 0; index1 < 3; index1++) {
	location = rand() % 2;
	
	switch(location)
	{
	    case 0: { /* vertical location */
		m = rand() % 9;
		n = rand() % 10;
		
		while(map[m][n] == 1 || map[m - 1][n - 1] == 1 || map[m - 1][n] == 1 ||
		      map[m - 1][n + 1] == 1 || map[m][n - 1] == 1 || map[m][n + 1] == 1 ||
		      map[m + 1][n - 1] == 1 || map[m + 1][n] == 1 || map[m + 1][n + 1] == 1 ||
		      map[m + 2][n - 1] == 1 || map[m + 2][n] == 1 || map[m + 2][n + 1] == 1) {
		    m = rand() % 9;
		    n = rand() % 10;
		}
		
		map[m][n] = 1;
		map[m + 1][n] = 1;
		
		break;
	    }
	    
	    case 1: { /* horizontal position */
		m = rand() % 10;
		n = rand() % 9;
		
		while(map[m][n] == 1 || map[m][n + 1] == 1 || map[m - 1][n - 1] == 1 ||
		      map[m - 1][n] == 1 || map[m - 1][n + 1] == 1 || map[m - 1][n + 2] == 1 ||
		      map[m][n - 1] == 1 || map[m][n + 2] == 1 || map[m + 1][n - 1] == 1 ||
		      map[m + 1][n] == 1 || map[m + 1][n + 1] == 1 || map[m + 1][n + 2] == 1) {
		    m = rand() % 10;
		    n = rand() % 9;
		}
		
		map[m][n] = 1;
		map[m][n + 1] = 1;
		
		break;
	    }
	}
    }
    
    /*
     * single-deck ship
     */
    for(index1 = 0; index1 < 4; index1++) {
	m = rand() % 10;
	n = rand() % 10;
	
	while(map[m][n] == 1 || map[m][n + 1] == 1 || map[m][n - 1] == 1 || map[m + 1][n] == 1 ||
	      map[m - 1][n] == 1 || map[m + 1][n + 1] == 1 || map[m + 1][n - 1] == 1 ||
	      map[m - 1][n + 1] == 1 || map[m - 1][n - 1] == 1) {
	    m = rand() % 10;
	    n = rand() % 10;
	}
    
    map[m][n] = 1;
    }
}

void cl_logic_merge_placement_map(placement server_map, map cl_map)
{
    int index1, index2;
    
    for(index1 = 0; index1 < 10; index1++) {
	for(index2 = 0; index2 < 10; index2++) {
		if(server_map[index1][index2] == 1){
			if(!cl_map[index1][index2])
				cl_map[index1][index2] = MAP_SHIP;
		}
	}
    }
}

int cl_logic_shot(int x, int y, enum srv_net_shot_result result, map cl_map, enum player_state *state)
{
    if(*state == ENEMY_TURN) {
	switch(result) {
	    case SRV_NET_HIT: {
		cl_map[y][x] = MAP_WOUNDED;
		
		break;
	    }
	    
	    case SRV_NET_KILL: {
		cl_logic_kill_ship(x, y, MAP_KILLED, cl_map);
		
		break;
	    }
	    
	    case SRV_NET_MISS: {
		cl_map[y][x] = MAP_MISS;
		*state = MY_TURN;
		
		break;
	    }
	    
	    case SRV_NET_END: {
		cl_logic_kill_ship(x, y, MAP_KILLED, cl_map);
		//return 1;
		
		break;
	    }
	}
    } else {
	if(*state == WAITING_TURN) {
	    switch(result) {
		case SRV_NET_HIT: {
		    cl_map[y][x] = MAP_WOUNDED;
		    *state = MY_TURN;
		    
		    break;
		}
		
		case SRV_NET_KILL: {
		    cl_logic_kill_ship(x, y, MAP_KILLED, cl_map);
		    *state = MY_TURN;
		    
		    break;
		}
		
		case SRV_NET_MISS: {
		    cl_map[y][x] = MAP_MISS;
		    *state = ENEMY_TURN;
		    
		    break;
		}
		
		case SRV_NET_END: {
		    cl_logic_kill_ship(x, y, MAP_KILLED, cl_map);
		    *state = MY_TURN;
		    //return 1;
		    
		    break;
		}
	    }
	}
    }
    
    return 0;
}

void cl_logic_kill_ship(int x, int y, int result, map cl_map)
{
    int index1, index2;
    int x1 = x, y1 = y;
    int left, right, up, down;
    int vertical = 0;
    int horizontal = 0;
    int one = 0;
    
    if(y == 9 && (cl_map[y1 - 1][x1] == 2)) {
	    down = y1;
	    cl_map[y1][x1] = result;
	    y1--;
	    
	    while(cl_map[y1][x1] == 2) {
		cl_map[y1][x1] = result;
		
		if(cl_map[y1 - 1][x1] != 2) {
		    up = y1;
		    break;
	        }
	        
		y1--;
	    }
	    
	    vertical = 1;
    } else {
	if(y1 == 0 && (cl_map[y1 + 1][x1] == 2)) {
		up = y1;
		cl_map[y1][x1] = result;
		y1++;
		
		while(cl_map[y1][x1] == 2) {
		    cl_map[y1][x1] = result;
		    
		    if(cl_map[y1+1][x1] != 2) {
			down = y1;
			break;
		    }
		    
		    y1++;
		}
		
		y1 = y;
		
	        vertical = 1;
	} else {
	    if(y1 != 0 && y1 != 9 && (cl_map[y1 - 1][x1] == 2 || cl_map[y1 + 1][x1] == 2)) {
		cl_map[y1][x1] = result;
		
		if(cl_map[y1 - 1][x1] == 2) {
		    y1--;
		    
		    while(cl_map[y1][x1] == 2) {
			cl_map[y1][x1] = result;
			up = y1;
			y1--;
			
			if(y1 == -1) {
			    break;
			}
		    }
		} else {
		    up = y1;
		}
		
		y1 = y;
		
		if(cl_map[y1 + 1][x1] == 2) {
		    y1++;
		    
		    while(cl_map[y1][x1] == 2) {
			cl_map[y1][x1] = result;
			down = y1;
			y++;
			
			if(y1 == 10) {
			    break;
			}
		    }
		} else {
		    down = y1;
		}
		
		y1 = y;
		
		vertical = 1;
	    }
	}
    }
    
    if(x1 == 9 && (cl_map[y1][x1 - 1] == 2)) {
	    right = x1;
	    cl_map[y1][x1] = result;
	    x1--;
	    
	    while(cl_map[y1][x1] == 2) {
		cl_map[y1][x1] = result;
		left = x1;
		
		if(cl_map[y1][x1 - 1] != 2) {
		    break;
		}
	        
		x1--;
	    }
	    
	    horizontal = 1;
    } else {
	if(x1 == 0 && (cl_map[y1][x1 + 1] == 2)) {
		left = x1;
		cl_map[y1][x1] = result;
		x1++;
		
		while(cl_map[y1][x1] == 2) {
		    cl_map[y1][x1] = result;
		    right = x1;
		    
		    if(cl_map[y1][x1 + 1] != 2) {
			break;
		    }
		    
		    x1++;
	        }
	    
		horizontal = 1;
	} else {
	    if(x1 != 0 && x1 != 10 && (cl_map[y1][x1 - 1] == 2 || cl_map[y1][x1 + 1] == 2)) {
		cl_map[y1][x1] = result;
		
		if(cl_map[y1][x1 - 1] == 2) {
		    x1--;
		    
		    while(cl_map[y1][x1] == 2) {
			cl_map[y1][x1] = result;
			left = x1;
			x1--;
			
			if(x1 == -1) {
			    break;
			}
		    }
		} else {
		    left = x1;
		}
		
		x1 = x;
		
		if(cl_map[y1][x1 + 1] == 2) {
		    x1++;
		    
		    while(cl_map[y1][x1] == 2) {
			cl_map[y1][x1] = result;
			right = x1;
			x1++;
			
			if(x1 == 10) {
			    break;
			}
		    }
		} else {
		    right = x1;
		}
		
		x1 = x;
		
		horizontal = 1;
	    }
	}
    }
    
    if(horizontal != 1 && vertical != 1) {
	one = 1;
    }
    
    if(one == 1) {
	cl_map[y][x] = result;
	
	for(index1 = y - 1; index1 < y+2; index1++) {
	    for(index2 = x - 1; index2 < x+2; index2++) {
		if(index2 == 10 || index2 == -1)
		    continue;
		
		if(index1 == 10 || index1 == -1)
		    continue;
		
		if((index1 == y) && (index2 == x))
		    continue;
		
		cl_map[index1][index2] = 4;
	    }
	}
    } else {
	if(horizontal == 1) {
	    if((left - 1) != -1)
		left--;
	    
	    if((right + 1) != 10)
		right++;
	    
	    if((y - 1) != -1) {
		up = y - 1;
	    } else {
		up = y;
	    }
	    
	    if((y + 1) != 10) {
		down = y + 1;
	    } else {
		down = y;
	    }
	    
	    for(index1 = up; index1 <= down; index1++) {
		for(index2 = left; index2 <= right; index2++) {
		    if(cl_map[index1][index2] == result)
			continue;
		    
		    cl_map[index1][index2] = 4;
		}
	    }
	} else {
	    if(vertical == 1) {
		if((up - 1) != -1)
		    up--;
		
		if((down + 1) != 10)
		    down++;
		
		if((x - 1) != -1) {
		    left = x - 1;
		} else {
		    left = x;
		}
		
		if((x + 1) != 10) {
		    right = x + 1;
		} else {
		    right = x;
		}
		
		for(index1 = left; index1 <= right; index1++) {
		    for(index2 = up; index2 <= down; index2++) {
			if(cl_map[index2][index1] == result)
			    continue;
			
			cl_map[index2][index1] = 4;
		    }
		}
	    }
	}
    }
}

int cl_logic_valid_shot(int x, int y, map cl_map, enum player_state *state)
{
    if(*state == MY_TURN) {
	if(cl_map[y][x] == MAP_EMPTY || cl_map[y][x] == MAP_SHIP) {
	    *state = WAITING_TURN;
	    return 1;
	}
	else
	    return 0;
    }
    else
	return 0;
}

char* cl_logic_convert_placement(placement fild1)
{
    char *fild2 = malloc(sizeof(map));
    
    memcpy(fild2, fild1, sizeof(map));
    
    return fild2;
}
