/* vim: set noexpandtab tabstop=4 shiftwidth=4 smartindent: */
#include "cl_logic.h"

void cl_logic_generate_placement(placement map[10][10])
{
    int index1, index2, n, m, location;
    
    for(index1 = 0; index1 < 10; index1++)
    {
	for(index2 = 0; index2 < 10; index2++)
	{
	    map[index1][index2] = 0;
	}
    }
    
    srand(time(NULL));
    
    /* the four-ship */
    location = rand() % 2;
    
    switch(location)
    {
	case 0: /* vertical location */
	{
	    m = rand() % 7;
	    n = rand() % 10;
	    
	    while((map[m][n] == 1) || (map[m + 1][n] == 1) || (map[m + 2][n] == 1) ||
	          (map[m + 3][n] == 1) || (map[m - 1][n - 1] == 1) || (map[m - 1][n] == 1) ||
	          (map[m - 1][n + 1] == 1) || (map[m][n - 1] == 1) || (map[m][n + 1] == 1) ||
	          (map[m + 1][n - 1] == 1) || (map[m + 1][n + 1] == 1) || (map[m + 2][n - 1] == 1) ||
	          (map[m + 2][n + 1] == 1) || (map[m + 3][n - 1] == 1) || (map[m + 3][n + 1] == 1) ||
	          (map[m + 4][n - 1] == 1) || (map[m + 4][n] == 1) || (map[m + 4][n + 1] == 1))
	    {
		m = rand() % 7;
		n = rand() % 10;
	    }
	    
	    map[m][n] = 1;
	    map[m + 1][n] = 1;
	    map[m + 2][n] = 1;
	    map[m + 3][n] = 1;
	    
	    break;
	}
	
	case 1: /* horizontal location */
	{
	    m = rand() % 10;
	    n = rand() % 7;
	    
	    while(map[m][n] == 1 || map[m][n + 1] == 1 || map[m][n + 2] == 1 || map[m][n + 3] == 1 ||
	          map[m - 1][n - 1] == 1 || map[m - 1][n] == 1 || map[m - 1][n + 1] == 1 || map[m - 1][n + 2] == 1 ||
	          map[m - 1][n + 3] == 1 || map[m - 1][n + 4] == 1 || map[m][n - 1] == 1 || map[m][n + 4] == 1 ||
	          map[m + 1][n - 1] == 1 || map[m + 1][n] == 1 || map[m + 1][n + 1] == 1 || map[m + 1][n + 2] == 1 ||
	          map[m + 1][n + 3] == 1 || map[m + 1][n + 4] == 1)
	    {
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
    
    /* three-deck ship */
    for(index1 = 0; index1 < 2; index1++)
    {
	location = rand() % 2;
	
	switch(location)
	{
	    case 0: /* vertical location */
	    {
		m = rand() % 8;
		n = rand() % 10;
		
		while(map[m][n] == 1 || map[m + 1][n] == 1 || map[m + 2][n] == 1 ||
		      map[m - 1][n - 1] == 1 || map[m - 1][n] == 1 || map[m - 1][n + 1] == 1 ||
		      map[m][n - 1] == 1 || map[m][n + 1] == 1 || map[m + 1][n - 1] == 1 ||
		      map[m + 1][n + 1] == 1 || map[m + 2][n - 1] == 1 || map[m + 2][n + 1] == 1 ||
		      map[m + 3][n - 1] == 1 || map[m + 3][n] == 1 || map[m + 3][n + 1] == 1)
		{
		    m = rand() % 8;
		    n = rand() % 10;
		}
		
		map[m][n] = 1;
		map[m + 1][n] = 1;
		map[m + 2][n] = 1;
		
		break;
	    }
	    
	    case 1: /* horizontal location */
	    {
		m = rand() % 10;
		n = rand() % 8;
		
		while(map[m][n] == 1 || map[m][n + 1] == 1 || map[m][n + 2] == 1 ||
		      map[m - 1][n - 1] == 1 || map[m - 1][n] == 1 || map[m - 1][n + 1] == 1 ||
		      map[m - 1][m + 2] == 1 || map[m - 1][n + 3] == 1 || map[m][n - 1] == 1 ||
		      map[m][n + 3] == 1 || map[m + 1][n - 1] == 1 || map[m + 1][n] == 1 ||
		      map[m + 1][n + 1] == 1 || map[m + 1][n + 2] == 1 || map[m + 1][n + 3] == 1)
		{
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
    
    /* double-decked ship */
    for(index1 = 0; index1 < 3; index1++)
    {
	location = rand() % 2;
	
	switch(location)
	{
	    case 0: /* vertical location */
	    {
		m = rand() % 9;
		n = rand() % 10;
		
		while(map[m][n] == 1 || map[m - 1][n - 1] == 1 || map[m - 1][n] == 1 ||
		      map[m - 1][n + 1] == 1 || map[m][n - 1] == 1 || map[m][n + 1] == 1 ||
		      map[m + 1][n - 1] == 1 || map[m + 1][n] == 1 || map[m + 1][n + 1] == 1 ||
		      map[m + 2][n - 1] == 1 || map[m + 2][n] == 1 || map[m + 2][n + 1] == 1)
		{
		    m = rand() % 9;
		    n = rand() % 10;
		}
		
		map[m][n] = 1;
		map[m + 1][n] = 1;
		
		break;
	    }
	    
	    case 1: /* horizontal position */
	    {
		m = rand() % 10;
		n = rand() % 9;
		
		while(map[m][n] == 1 || map[m][n + 1] == 1 || map[m - 1][n - 1] == 1 ||
		      map[m - 1][n] == 1 || map[m - 1][n + 1] == 1 || map[m - 1][n + 2] == 1 ||
		      map[m][n - 1] == 1 || map[m][n + 2] == 1 || map[m + 1][n - 1] == 1 ||
		      map[m + 1][n] == 1 || map[m + 1][n + 1] == 1 || map[m + 1][n + 2] == 1)
		{
		    m = rand() % 10;
		    n = rand() % 9;
		}
		
		map[m][n] = 1;
		map[m][n + 1] = 1;
		
		break;
	    }
	}
    }
    
    /* single-deck ship */
    for(index1 = 0; index1 < 4; index1++)
    {
	m = rand() % 10;
	n = rand() % 10;
	
	while(map[m][n] == 1 || map[m][n + 1] == 1 || map[m][n - 1] == 1 || map[m + 1][n] == 1 ||
	      map[m - 1][n] == 1 || map[m + 1][n + 1] == 1 || map[m + 1][n - 1] == 1 ||
	      map[m - 1][n + 1] == 1 || map[m - 1][n - 1] == 1)
	{
	    m = rand() % 10;
	    n = rand() % 10;
	}
    
    map[m][n] = 1;
    }
}

void cl_logic_merge_placement_map(placament server_map[10][10], placement map[10][10])
{
    int index1, index2;
    
    /*placement temp_map[10][10];*/
    
    for(index1 = 0; index1 < 10; index1++)
    {
	for(index2 = 0; index2 < 10; index2++)
	{
	    /*temp_map[index1][index2] = server_map[index1][index2];
	    temp_map = map[index1][index2];*/
	    map[index1][index2] = server_map[index1][index2] + map[index1][index2];
	}
    }
    
    /*return temp_map;*/
}

void cl_logic_shot(int x, int y, int result, placement map[10][10], enum player_state *state)
{
    if(state == ENEMY_TURN)
    {
	switch(result)
	{
	    case 2:
	    {
		map[y][x] = result;
		
		break;
	    }
	    
	    case 3:
	    {
		cl_logic_kill_ship(x, y, result, map);
		
		break;
	    }
	    
	    case 4:
	    {
		map[y][x] = result;
		state = MY_TURN;
		
		break;
	    }
	}
    }
    else
    {
	if(state == WAITING_TURN)
	{
	    switch(result)
	    {
		case 2:
		{
		    map[y][x] = result;
		    
		    break;
		}
		
		case 3:
		{
		    cl_logic_kill_ship(x, y, result, map);
		    
		    break;
		}
		
		case 4:
		{
		    map[y][x] = result;
		    state = ENEMY_TURN;
		    
		    break;
		}
	    }
	}
    }
}

void cl_logic_kill_ship(int x, int y, int result, placement map[10][10])
{
    int index1, index2;
    int x1 = x, y1 = y;
    int left, right, up, down;
    int vertical = 0;
    int horizontal = 0;
    int one = 0;
    
    if(y == 9 && (map[y1 - 1][x1] == 2))/* || map[y1 - 1][x1] =! 2))*/
    {
	/*if(map[y1 - 1][x1] != 2)
	{
	    one = 1;
	    map[y][x] = result;
	}
	else
	{*/
	    down = y1;
	    map[y1][x1] = result;
	    y1--;
	    
	    while(map[y1][x1] == 2)
	    {
		map[y1][x1] = result;
		
		if(map[y1 - 1][x1] != 2)
		{
		    up = y1;
		    break;
	        }
	        
		y1--;
	    }
	    
	    vertical = 1;
	/*}*/
    }
    else
    {
	if(y1 == 0 && (map[y1 + 1][x1] == 2))/* || map[y1 + 1][x1] != 2))*/
	{
	    /*if(map[y1 + 1][x1] != 2)
	    {
		one = 1;
		map[y][x] = result;
	    }
	    else
	    {*/
		up = y1;
		map[y1][x1] = result;
		y1++;
		
		while(map[y1][x1] == 2)
		{
		    map[y1][x1] = result;
		    
		    if(map[y1+1][n1] != 2)
		    {
			down = y1;
			break;
		    }
		    
		    y1++;
		}
		
	        vertical = 1;
	    /*}*/
	}
	else
	{
	    if(y1 != 0 && y1 != 9 && (map[y1 - 1][x1] == 2 || map[y1 + 1][x1] == 2))/* || map[y1 - 1][x1] != 2 || map[y1 + 1][x1] != 2))*/
	    {
		map[y1][x1] = result;
		
		if(map[y1 - 1][x1] == 2)
		{
		    y1--;
		    
		    while(map[y1][x1] == 2)
		    {
			map[y1][x1] = result;
			up = y1;
			y1--;
			
			if(y1 == -1)
			{
			    y1 = y;
			    break;
			}
		    }
		}
		else
		{
		    up = y1;
		}
		
		if(map[y1 + 1][x1] == 2)
		{
		    y++;
		    
		    while(map[y1][x1] == 2)
		    {
			map[y1][x1] = result;
			down = y1
			y++;
			
			if(y1 == 10)
			{
			    y1 = y;
			    break;
			}
		    }
		}
		
		vertical = 1;
	    }
	}
    }
    
    if(x1 == 9 && (map[y1][x1 - 1] == 2))/* || map[y1][x1 - 1] != 2))*/
    {
	/*if(map[y1][x1 - 1] != 2)
	{
	    one = 1;
	    map[y][x] = result;
	}
	else
	{*/
	    right = x1;
	    map[y1][x1] = result;
	    x1--;
	    
	    while(map[y1][x1] == 2)
	    {
		map[y1][x1] = result;
		left = x1;
		
		if(map[y1][x1 - 1] != 2)
		{
		    left = x1;
		    break;
		}
	        
		x1--;
	    }
	    
	    horizontal = 1;
	/*}*/
    }
    else
    {
	if(x1 == 0 && (map[y1][x1 + 1] == 2))/* || map[y1][x1 + 1] != 2))*/
	{
	    /*if(map[y1][x1 + 1] != 2)
	    {
		one = 1;
		map[y][x] = result;
	    }
	    else
	    {*/
		left = x1;
		map[y1][x1] = result;
		x1++;
		
		while(map[y1][x1] == 2)
		{
		    map[y1][x1] = result;
		    
		    if(map[y1][x1 + 1] != 2)
		    {
			right = x1;
			break;
		    }
		    
		    x1++;
	        }
	    
		horizontal = 1;
	    /*}*/
	}
	else
	{
	    if(x1 != 0 && x1 != 10 && (map[y1][x1 - 1] == 2 || map[y1][x1 + 1] == 2))
	    {
		map[y1][x1] = result;
		
		if(map[y1][x1 - 1] == 2)
		{
		    x1--;
		    
		    while(map[y1][x1] == 2)
		    {
			map[y1][x1] = result;
			left = x1;
			x1--;
			
			if(x1 == -1)
			{
			    x1 = x;
			    break;
			}
		    }
		}
		else
		{
		    left = x1;
		}
		
		if(map[y1][x1 + 1] == 2)
		{
		    x1++;
		    
		    while(map[y1][x1] == 2)
		    {
			map[y1][x1] = result;
			right = x1;
			x1++;
			
			if(x1 == 10)
			{
			    x1 = x;
			    break;
			}
		    }
		}
		
		horizontal = 1;
	    }
	}
    }
    
    /*if()*/
    
    if(one == 1)
    {
	for(index1 = y - 1; index1 < y+2; index1++)
	{
	    for(index2 = x - 1; index2 < x+2; index2++)
	    {
		if(index2 == 10 || index2 == -1)
		    continue;
		
		if(index1 == 10 || index1 == -1)
		    continue;
		
		if((index1 == y) && (index2 == x))
		    continue;
		
		map[index1][index2] = 4;
	    }
	}
    }
}

int cl_logic_valid_shot(int x, int y, map map[10][10], enum player_state *state)
{
    if(state == MY_TURN)
    {
	if(map[y][x] == 0 || map[y][x] == 1)
	{
	    state = WAITING_TURN;
	    return 1;
	}
	else
	    return 0;
    }
    else
	return 0;
}