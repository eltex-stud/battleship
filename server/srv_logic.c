/* vim: set noexpandtab tabstop=4 shiftwidth=4 smartindent: */
#include <stdio.h>
#include <stdlib.h>

#include <server/srv_logic.h>

/**
 * 
 * @struct srv_logic_map stores the coordinates of ship
 * @each ship your item structure
 * @variable counts falling
 *
 */

struct srv_logic_map {
	int ship1_1[3][1];
	int ship1_2[3][1];
	int ship1_3[3][1];
	int ship1_4[3][1];
	int ship2_1[3][2];
	int ship2_2[3][2];
	int ship2_3[3][2];
	int ship3_1[3][3];
	int ship3_2[3][3];
	int ship4_1[3][4];
	int count;
};

/*
 *
 * @function placement_to_map creatrs and fills structure srv_logic_map
 * @placement[10][10] a binary map
 *
 */

struct srv_logic_map *placement_to_map(int placement[10][10]) {
	int idx, jdx;

/*
 * @create a pointer c_t_m and allocate memory for it
 */

	struct srv_logic_map *c_t_m;
	c_t_m = (struct srv_logic_map *)malloc(sizeof(struct srv_logic_map));

/*
 * @create a test array and fill it with zeros
 */

	int placement_test[15][15];
	for (idx = 0; idx < 15; idx++) {
		for (jdx = 0; jdx < 15; jdx++) {
			placement_test[idx][jdx] = 0;
		}
	}

/*
 *
 * @we bring an array placement into an array placement_test
 * @the beginning of the main body must be in the coordinate idx = 1, jdx = 1 
 * array placement_test
 *
 *
 * @ 	000000000000000
 * @ 	011111111110000
 * @ 	011111111110000
 * @ 	011111111110000
 * @ 	011111111110000
 * @ 	011111111110000
 * @ 	011111111110000
 * @ 	011111111110000
 * @ 	011111111110000
 * @ 	011111111110000
 * @ 	011111111110000
 * @ 	000000000000000
 * @ 	000000000000000
 * @ 	000000000000000
 * @ 	000000000000000
 *
 *
 * @it is necessery tobe able to use the general search algorithm ship
 * @coordinate the beginning of the ship does not matter
 *
 */

	for (idx = 1; idx < 11; idx++) {
		for (jdx = 1; jdx < 11; jdx++) {
			placement_test[idx][jdx] = placement[idx - 1][jdx - 1];
		}
	}

/*
 *
 * @begin the search for ships
 * @one search cycle for each type of ship
 * @derived coordinates subtract 1 and inserted in the appropriate 
 * element of the structure
 *
 */

/*
 * @long search ships in one deck
 */

	for (idx = 0; idx < 15; idx++) {
		for (jdx = 0; jdx < 15; jdx++) {
			if (placement_test[idx][jdx] == 1) {
				if (placement_test[idx][jdx - 1] != 1 
					 && placement_test[idx - 1][jdx] != 1
					 && placement_test[idx + 1][jdx] == 0 
					 && placement_test[idx][jdx + 1] == 0) {
					if (c_t_m->ship1_1[1][1] == 0) {
						c_t_m->ship1_1[1][1] = idx - 1;
						c_t_m->ship1_1[2][1] = jdx - 1;
						c_t_m->ship1_1[3][1] = 1;
						jdx++;
					}
				}
			}
			if (placement_test[idx][jdx] == 1) {
				if (placement_test[idx][jdx - 1] != 1 
					 && placement_test[idx - 1][jdx] != 1
					 && placement_test[idx + 1][jdx] == 0 
					 && placement_test[idx][jdx + 1] == 0) {
					if (c_t_m->ship1_2[1][1] == 0) {
						c_t_m->ship1_2[1][1] = idx - 1;
						c_t_m->ship1_2[2][1] = jdx - 1;
						c_t_m->ship1_2[3][1] = 1;
						jdx++;
					}
				}
			}
			if (placement_test[idx][jdx] == 1) {
				if (placement_test[idx][jdx - 1] != 1 
					 && placement_test[idx - 1][jdx] != 1
					 && placement_test[idx + 1][jdx] == 0 
					 && placement_test[idx][jdx + 1] == 0) {
					if (c_t_m->ship1_3[1][1] == 0) {
						c_t_m->ship1_3[1][1] = idx - 1;
						c_t_m->ship1_3[2][1] = jdx - 1;
						c_t_m->ship1_3[3][1] = 1;
						jdx++;
					}
				}
			}
			if (placement_test[idx][jdx] == 1) {
				if (placement_test[idx][jdx - 1] != 1 
					 && placement_test[idx - 1][jdx] != 1
					 && placement_test[idx + 1][jdx] == 0 
					 && placement_test[idx][jdx + 1] == 0) {
					if (c_t_m->ship1_4[1][1] == 0) {
						c_t_m->ship1_4[1][1] = idx - 1;
						c_t_m->ship1_4[2][1] = jdx - 1;
						c_t_m->ship1_4[3][1] = 1;
					}
				}
			}
		}
	}

/*
 * @search ships longest in two decks
 */

	for (idx = 0; idx < 15; idx++) {
		for (jdx = 0; jdx < 15; jdx++) {
			if (placement_test[idx][jdx] == 1) {
				if (placement_test[idx - 1][jdx] != 1 
					&& placement_test[idx][jdx - 1] != 1
					&& placement_test[idx][jdx + 1] == 1 
					&& placement_test[idx][jdx + 2] == 0
					&& placement_test[idx + 1][jdx] == 0) {
					c_t_m->ship2_1[1][1] = idx - 1;
					c_t_m->ship2_1[2][1] = jdx - 1;
					c_t_m->ship2_1[1][2] = idx - 1;
					c_t_m->ship2_1[2][2] = jdx - 1 + 1;
					c_t_m->ship2_1[3][1] = 1;
					c_t_m->ship2_1[3][2] = 1;
					jdx++;
					} else { 
						if (placement_test[idx - 1][jdx] != 1 
							&& placement_test[idx][jdx - 1] != 1
							&& placement_test[idx + 1][jdx] == 1 
							&& placement_test[idx + 2][jdx] == 0
							&& placement_test[idx][jdx + 1] == 0) {
							c_t_m->ship2_1[1][1] = idx - 1;
							c_t_m->ship2_1[2][1] = jdx - 1;
							c_t_m->ship2_1[1][2] = idx - 1 + 1;
							c_t_m->ship2_1[2][2] = jdx - 1;
							c_t_m->ship2_1[3][1] = 1;
							c_t_m->ship2_1[3][2] = 1;
							jdx++;
						}
					}
				}
			if (placement_test[idx][jdx] == 1) {
				if (placement_test[idx - 1][jdx] != 1 
					&& placement_test[idx][jdx - 1] != 1
					&& placement_test[idx][jdx + 1] == 1 
					&& placement_test[idx][jdx + 2] == 0
					&& placement_test[idx + 1][jdx] == 0) {
					c_t_m->ship2_2[1][1] = idx - 1;
					c_t_m->ship2_2[2][1] = jdx - 1;
					c_t_m->ship2_2[1][2] = idx - 1;
					c_t_m->ship2_2[2][2] = jdx - 1 + 1;
					c_t_m->ship2_2[3][1] = 1;
					c_t_m->ship2_2[3][2] = 1;
					jdx++;
					} else { 
						if (placement_test[idx - 1][jdx] != 0 
							&& placement_test[idx][jdx - 1] != 0
							&& placement_test[idx + 1][jdx] == 1 
							&& placement_test[idx + 2][jdx] == 0
							&& placement_test[idx][jdx + 1] == 0) {
							c_t_m->ship2_2[1][1] = idx - 1;
							c_t_m->ship2_2[2][1] = jdx - 1;
							c_t_m->ship2_2[1][2] = idx - 1 + 1;
							c_t_m->ship2_2[2][2] = jdx - 1;
							c_t_m->ship2_2[3][1] = 1;
							c_t_m->ship2_2[3][2] = 1;
							jdx++;
						} 
					}
				}
			if (placement_test[idx][jdx] == 1) {
				if (placement_test[idx - 1][jdx] != 1
					&& placement_test[idx][jdx - 1] != 1
					&& placement_test[idx][jdx + 1] == 1 
					&& placement_test[idx][jdx + 2] == 0
					&& placement_test[idx + 1][jdx] == 0) {
					c_t_m->ship2_3[1][1] = idx - 1;
					c_t_m->ship2_3[2][1] = jdx - 1;
					c_t_m->ship2_3[1][2] = idx - 1;
					c_t_m->ship2_3[2][2] = jdx - 1 + 1;
					c_t_m->ship2_3[3][1] = 1;
					c_t_m->ship2_3[3][2] = 1;
					} else { 
						if (placement_test[idx - 1][jdx] != 1
							&& placement_test[idx][jdx - 1] != 1
							&& placement_test[idx + 1][jdx] == 1 
							&& placement_test[idx + 2][jdx] == 0
							&& placement_test[idx][jdx + 1] == 0) {
							c_t_m->ship2_3[1][1] = idx - 1;
							c_t_m->ship2_3[2][1] = jdx - 1;
							c_t_m->ship2_3[1][2] = idx - 1 + 1;
							c_t_m->ship2_3[2][2] = jdx - 1;
							c_t_m->ship2_3[3][1] = 1;
							c_t_m->ship2_3[3][2] = 1;
						}
					}
				}
			}
		}

/*
 * @search in three long ships deck
 */

	for (idx = 0; idx < 15; idx++) {
		for (jdx = 0; jdx < 15; jdx++) {
			if (placement_test[idx][jdx] == 1) {
				if (placement_test[idx - 1][jdx] != 1
					&& placement_test[idx][jdx - 1] != 1
					&& placement_test[idx][jdx + 1] == 1
					&& placement_test[idx][jdx + 2] == 1
					&& placement_test[idx][jdx + 3] == 0
					&& placement_test[idx + 1][jdx] == 0) {
					c_t_m->ship3_1[1][1] = idx - 1;
					c_t_m->ship3_1[2][1] = jdx - 1;
					c_t_m->ship3_1[1][2] = idx - 1;
					c_t_m->ship3_1[2][2] = jdx - 1 + 1;
					c_t_m->ship3_1[1][3] = idx - 1;
					c_t_m->ship3_1[2][3] = jdx - 1 + 2;
					c_t_m->ship3_1[3][1] = 1;
					c_t_m->ship3_1[3][2] = 1;
					c_t_m->ship3_1[3][3] = 1;
					jdx++;
					} else {
						if (placement_test[idx - 1][jdx] != 1
							&& placement_test[idx][jdx - 1] != 1
							&& placement_test[idx][jdx + 1] == 0
							&& placement_test[idx + 1][jdx] == 1
							&& placement_test[idx + 2][jdx] == 1
							&& placement_test[idx + 3][jdx] == 0) {
							c_t_m->ship3_1[1][1] = idx - 1;
							c_t_m->ship3_1[2][1] = jdx - 1;
							c_t_m->ship3_1[1][2] = idx - 1 + 1;
							c_t_m->ship3_1[2][2] = jdx - 1;
							c_t_m->ship3_1[1][3] = idx - 1 + 2;
							c_t_m->ship3_1[2][3] = jdx - 1;
							c_t_m->ship3_1[3][1] = 1;
							c_t_m->ship3_1[3][2] = 1;
							c_t_m->ship3_1[3][3] = 1;
							jdx++;
						}
					}
				}
			if (placement_test[idx][jdx] == 1) {
				if (placement_test[idx - 1][jdx] != 1
					&& placement_test[idx][jdx - 1] != 1
					&& placement_test[idx][jdx + 1] == 1
					&& placement_test[idx][jdx + 2] == 1
					&& placement_test[idx][jdx + 3] == 0
					&& placement_test[idx + 1][jdx] == 0) {
					c_t_m->ship3_2[1][1] = idx - 1;
					c_t_m->ship3_2[2][1] = jdx - 1;
					c_t_m->ship3_2[1][2] = idx - 1;
					c_t_m->ship3_2[2][2] = jdx - 1 + 1;
					c_t_m->ship3_2[1][3] = idx - 1;
					c_t_m->ship3_2[2][3] = jdx - 1 + 2;
					c_t_m->ship3_2[3][1] = 1;
					c_t_m->ship3_2[3][2] = 1;
					c_t_m->ship3_2[3][3] = 1;
					} else {
						if (placement_test[idx - 1][jdx] != 1
							&& placement_test[idx][jdx - 1] != 1
							&& placement_test[idx][jdx + 1] == 0
							&& placement_test[idx + 1][jdx] == 1
							&& placement_test[idx + 2][jdx] == 1
							&& placement_test[idx + 3][jdx] == 0) {
							c_t_m->ship3_2[1][1] = idx - 1;
							c_t_m->ship3_2[2][1] = jdx - 1;
							c_t_m->ship3_2[1][2] = idx - 1 + 1;
							c_t_m->ship3_2[2][2] = jdx - 1;
							c_t_m->ship3_2[1][3] = idx - 1 + 2;
							c_t_m->ship3_2[2][3] = jdx - 1;
							c_t_m->ship3_2[3][1] = 1;
							c_t_m->ship3_2[3][2] = 1;
							c_t_m->ship3_2[3][3] = 1;
						}
					}
				}
			}
		}

/*
 * @searc long ship four decks
 */

	for (idx = 0; idx < 15; idx++) {
		for (jdx = 0; jdx < 15; jdx++) {
			if (placement[idx][jdx] == 1) {
				if (placement_test[idx - 1][jdx] != 1
					&& placement_test[idx][jdx - 1] != 1
					&& placement_test[idx + 1][jdx] == 0
					&& placement_test[idx][jdx + 1] == 1
					&& placement_test[idx][jdx + 2] == 1
					&& placement_test[idx][jdx + 3] == 1
					&& placement_test[idx][jdx + 4] == 0) {
					c_t_m->ship4_1[1][1] = idx - 1;
					c_t_m->ship4_1[2][1] = jdx - 1;
					c_t_m->ship4_1[1][2] = idx - 1;
					c_t_m->ship4_1[2][2] = jdx - 1 + 1;
					c_t_m->ship4_1[1][3] = idx - 1;
					c_t_m->ship4_1[2][3] = jdx - 1 + 2;
					c_t_m->ship4_1[1][4] = idx - 1;
					c_t_m->ship4_1[2][4] = jdx - 1 + 3;
					c_t_m->ship4_1[3][1] = 1;
					c_t_m->ship4_1[3][2] = 1;
					c_t_m->ship4_1[3][3] = 1;
					c_t_m->ship4_1[3][4] = 1;
					} else {
						if (placement_test[idx - 1][jdx] != 1
							&& placement_test[idx][jdx - 1] != 1
							&& placement_test[idx + 1][jdx] == 1
							&& placement_test[idx][jdx + 1] == 0
							&& placement_test[idx + 2][jdx] == 1
							&& placement_test[idx + 3][jdx] == 1
							&& placement_test[idx + 4][jdx] == 1) {
							c_t_m->ship4_1[1][1] = idx - 1;
							c_t_m->ship4_1[2][1] = jdx - 1;
							c_t_m->ship4_1[1][2] = idx - 1 + 1;
							c_t_m->ship4_1[2][2] = jdx - 1;
							c_t_m->ship4_1[1][3] = idx - 1 + 2;
							c_t_m->ship4_1[2][3] = jdx - 1;
							c_t_m->ship4_1[1][4] = idx - 1 + 3;
							c_t_m->ship4_1[2][4] = jdx - 1;
							c_t_m->ship4_1[3][1] = 1;
							c_t_m->ship4_1[3][2] = 1;
							c_t_m->ship4_1[3][3] = 1;
							c_t_m->ship4_1[3][4] = 1;
						}
					}
				}
			}
		}

/*
 * @zeroing test array
 */

	for (idx = 0; idx < 15; idx++) {
		for (jdx = 0; jdx < 15; jdx++) {
			placement_test[idx][jdx] = 0;
		}
	}

/*
 * @return structure
 */

	return c_t_m;
}

/*
 *
 * @function make_shot takes the shot structure coordinates 
 * and structure coordinates of ships
 * @function make_shot returns a shot and games
 *
 */

enum srv_logic_shot_result make_shot(struct srv_logic_map *c_t_m, 
		struct srv_logic_shot *shot) {

	/*
	 *
	 * @create a shot list of result
	 *
	 */

	enum srv_logic_shot_result result_shot;

	/*
	 * @default result does not hit the target
	 */

	result_shot = SRV_LOGIC_RESULT_MISS;

	/*
	 * @begin the test sequence coordinates are recived
	 */

	if (c_t_m->ship1_1[1][1] == shot->x 
			&& c_t_m->ship1_1[2][1] == shot->y) {
		result_shot = SRV_LOGIC_RESULT_KILL;

		c_t_m->ship1_1[3][1] = 3;
		c_t_m->count = c_t_m->count + 1;
	} 
	if (c_t_m->ship1_2[1][1] == shot->x 
			&& c_t_m->ship1_1[2][1] == shot->y) {
		result_shot = SRV_LOGIC_RESULT_KILL;
		c_t_m->ship1_1[3][1] = 3;
		c_t_m->count = c_t_m->count + 1;
	} 
	if (c_t_m->ship1_3[1][1] == shot->x 
			&& c_t_m->ship1_1[2][1] == shot->y) {
		result_shot = SRV_LOGIC_RESULT_KILL;
		c_t_m->ship1_1[3][1] = 3;
		c_t_m->count = c_t_m->count + 1;
	} 
	if (c_t_m->ship1_4[1][1] == shot->x 
			&& c_t_m->ship1_1[2][1] == shot->y) {
		result_shot = SRV_LOGIC_RESULT_KILL;
		c_t_m->ship1_1[3][1] = 3;
		c_t_m->count = c_t_m->count + 1;
	} 

	if (c_t_m->ship2_1[1][1] == shot->x
			&& c_t_m->ship2_1[2][1] == shot->y) {
		if (c_t_m->ship2_1[3][2] == 2) {
			c_t_m->ship2_1[3][1] = 3;
			c_t_m->ship2_1[3][2] = 3;
			result_shot = SRV_LOGIC_RESULT_KILL;
			c_t_m->count = c_t_m->count + 1;
		} else {
			c_t_m->ship2_1[3][1] = 2;
			result_shot = SRV_LOGIC_RESULT_HIT;
			c_t_m->count = c_t_m->count + 1;
		}
	} 
	if (c_t_m->ship2_1[1][2] == shot->x
			&& c_t_m->ship2_1[2][2] == shot->y) {
		if (c_t_m->ship2_1[3][1] == 2) {
			c_t_m->ship2_1[3][2] = 3;
			c_t_m->ship2_1[3][1] = 3;
			result_shot = SRV_LOGIC_RESULT_KILL;
			c_t_m->count = c_t_m->count + 1;
		} else {
			c_t_m->ship2_1[3][2] = 2;
			result_shot = SRV_LOGIC_RESULT_HIT;
			c_t_m->count = c_t_m->count + 1;
		}
	}
	if (c_t_m->ship2_2[1][1] == shot->x
			&& c_t_m->ship2_2[2][1] == shot->y) {
		if (c_t_m->ship2_2[3][2] == 2) {

			c_t_m->ship2_2[3][1] = 3;
			c_t_m->ship2_2[3][2] = 3;
			result_shot = SRV_LOGIC_RESULT_KILL;
			c_t_m->count = c_t_m->count + 1;
		} else {
			c_t_m->ship2_2[3][1] = 2;
			result_shot = SRV_LOGIC_RESULT_HIT;
			c_t_m->count = c_t_m->count + 1;
		}
	} 
	if (c_t_m->ship2_2[1][2] == shot->x
			&& c_t_m->ship2_2[2][2] == shot->y) {
		if (c_t_m->ship2_2[3][1] == 2) {
			c_t_m->ship2_2[3][2] = 3;
			c_t_m->ship2_2[3][1] = 3;
			result_shot = SRV_LOGIC_RESULT_KILL;
			c_t_m->count = c_t_m->count + 1;
		} else {
			c_t_m->ship2_2[3][2] = 2;
			result_shot = SRV_LOGIC_RESULT_HIT;
			c_t_m->count = c_t_m->count + 1;
		} 
	}
	if (c_t_m->ship2_3[1][1] == shot->x
			&& c_t_m->ship2_3[2][1] == shot->y) {
		if (c_t_m->ship2_3[3][2] == 2) {
			c_t_m->ship2_3[3][1] = 3;
			c_t_m->ship2_3[3][2] = 3;
			result_shot = SRV_LOGIC_RESULT_KILL;
			c_t_m->count = c_t_m->count + 1;
		} else {
			c_t_m->ship2_3[3][1] = 2;
			result_shot = SRV_LOGIC_RESULT_HIT;
			c_t_m->count = c_t_m->count + 1;
		} 
	} 
	if (c_t_m->ship2_3[1][2] == shot->x
			&& c_t_m->ship2_3[2][2] == shot->y) {
		if (c_t_m->ship2_3[3][1] == 2) {
			c_t_m->ship2_3[3][2] = 3;
			c_t_m->ship2_3[3][1] = 3;
			result_shot = SRV_LOGIC_RESULT_KILL;
			c_t_m->count = c_t_m->count + 1;
		} else {
			c_t_m->ship2_3[3][2] = 2;
			result_shot = SRV_LOGIC_RESULT_HIT;
			c_t_m->count = c_t_m->count + 1;
		} 
	}


	if (c_t_m->ship3_1[1][1] == shot->x
			&& c_t_m->ship3_1[2][1] == shot->y) {
		if (c_t_m->ship3_1[3][2] == 2
				&& c_t_m->ship3_1[3][3] == 2) {
			c_t_m->ship3_1[3][1] = 3;
			c_t_m->ship3_1[3][2] = 3;
			c_t_m->ship3_1[3][3] = 3;
			result_shot = SRV_LOGIC_RESULT_KILL;
			c_t_m->count = c_t_m->count + 1;
		} else {
			c_t_m->ship3_1[3][1] = 2;
			result_shot = SRV_LOGIC_RESULT_HIT;
			c_t_m->count = c_t_m->count + 1;
		}
	} 
	if (c_t_m->ship3_1[1][2] == shot->x
			&& c_t_m->ship3_1[2][2] == shot->y) {
		if (c_t_m->ship3_1[3][1] == 2
				&& c_t_m->ship3_1[3][3] == 2) {
			c_t_m->ship3_1[3][1] = 3;
			c_t_m->ship3_1[3][2] = 3;
			c_t_m->ship3_1[3][3] = 3;
			result_shot = SRV_LOGIC_RESULT_KILL;
			c_t_m->count = c_t_m->count + 1;
		} else {
			c_t_m->ship3_1[3][2] = 2;
			result_shot = SRV_LOGIC_RESULT_HIT;
			c_t_m->count = c_t_m->count + 1;
		}
	}
	if (c_t_m->ship3_1[1][3] == shot->x
			&& c_t_m->ship3_1[2][3] == shot->y) {
		if (c_t_m->ship3_1[3][1] == 2
				&& c_t_m->ship3_1[3][2] == 2) {
			c_t_m->ship3_1[3][1] = 3;
			c_t_m->ship3_1[3][2] = 3;
			c_t_m->ship3_1[3][3] = 3;
			result_shot = SRV_LOGIC_RESULT_KILL;
			c_t_m->count = c_t_m->count + 1;
		} else {
			c_t_m->ship3_1[3][3] = 2;
			result_shot = SRV_LOGIC_RESULT_HIT;
			c_t_m->count = c_t_m->count + 1;
		}
	}

	if (c_t_m->ship3_2[1][1] == shot->x
			&& c_t_m->ship3_2[2][1] == shot->y) {
		if (c_t_m->ship3_2[3][2] == 2
				&& c_t_m->ship3_2[3][3] == 2) {
			c_t_m->ship3_2[3][1] = 3;
			c_t_m->ship3_2[3][2] = 3;
			c_t_m->ship3_2[3][3] = 3;
			result_shot = SRV_LOGIC_RESULT_KILL;
			c_t_m->count = c_t_m->count + 1;
		} else {
			c_t_m->ship3_2[3][1] = 2;
			result_shot = SRV_LOGIC_RESULT_HIT;
			c_t_m->count = c_t_m->count + 1;
		}
	}
	if (c_t_m->ship3_2[1][2] == shot->x
			&& c_t_m->ship3_2[2][2] == shot->y) {
		if (c_t_m->ship3_2[3][1] == 2
				&& c_t_m->ship3_2[3][3] == 2) {
			c_t_m->ship3_2[3][1] = 3;
			c_t_m->ship3_2[3][2] = 3;
			c_t_m->ship3_2[3][3] = 3;
			result_shot = SRV_LOGIC_RESULT_KILL;
			c_t_m->count = c_t_m->count + 1;
		} else {
			c_t_m->ship3_2[3][2] = 2;
			result_shot = SRV_LOGIC_RESULT_HIT;
			c_t_m->count = c_t_m->count + 1;
		} 
	}
	if (c_t_m->ship3_2[1][3] == shot->x
			&& c_t_m->ship3_2[2][3] == shot->y) {
		if (c_t_m->ship3_2[3][1] == 2
				&& c_t_m->ship3_1[3][2] == 2) {
			c_t_m->ship3_2[3][1] = 3;
			c_t_m->ship3_2[3][2] = 3;
			c_t_m->ship3_2[3][3] = 3;
			result_shot = SRV_LOGIC_RESULT_KILL;
			c_t_m->count = c_t_m->count + 1;
		} else {
			c_t_m->ship3_2[3][3] = 2;
			result_shot = SRV_LOGIC_RESULT_HIT;
			c_t_m->count = c_t_m->count + 1;
		}
	}

	if (c_t_m->ship4_1[1][1] == shot->x
			&& c_t_m->ship4_1[2][1] == shot->y) {
		if (c_t_m->ship4_1[3][2] == 2
				&& c_t_m->ship4_1[3][3] == 2
				&& c_t_m->ship4_1[3][4] == 2) {
			c_t_m->ship4_1[3][1] = 3;
			c_t_m->ship4_1[3][2] = 3;
			c_t_m->ship4_1[3][3] = 3;
			c_t_m->ship4_1[3][4] = 3;
			result_shot = SRV_LOGIC_RESULT_KILL;
			c_t_m->count = c_t_m->count + 1;
		} else {
			c_t_m->ship4_1[3][1] = 2;
			result_shot = SRV_LOGIC_RESULT_HIT;
			c_t_m->count = c_t_m->count + 1;
		}
	}
	if (c_t_m->ship4_1[1][2] == shot->x
			&& c_t_m->ship4_1[2][2] == shot->y) {
		if (c_t_m->ship4_1[3][1] == 2
				&& c_t_m->ship4_1[3][3] == 2
				&& c_t_m->ship4_1[3][4] == 2) {
			c_t_m->ship4_1[3][1] = 3;
			c_t_m->ship4_1[3][2] = 3;
			c_t_m->ship4_1[3][3] = 3;
			c_t_m->ship4_1[3][4] = 3;
			result_shot = SRV_LOGIC_RESULT_KILL;
			c_t_m->count = c_t_m->count + 1;
		} else {
			c_t_m->ship4_1[3][2] = 2;
			result_shot = SRV_LOGIC_RESULT_HIT;
			c_t_m->count = c_t_m->count + 1;
		}
	}
	if (c_t_m->ship4_1[1][3] == shot->x
			&& c_t_m->ship4_1[2][3] == shot->y) {
		if (c_t_m->ship4_1[3][1] == 2
				&& c_t_m->ship4_1[3][2] == 2
				&& c_t_m->ship4_1[3][4] == 2) {
			c_t_m->ship4_1[3][1] = 3;
			c_t_m->ship4_1[3][2] = 3;
			c_t_m->ship4_1[3][3] = 3;
			c_t_m->ship4_1[3][4] = 3;
			result_shot = SRV_LOGIC_RESULT_KILL;
			c_t_m->count = c_t_m->count + 1;
		} else {
			c_t_m->ship4_1[3][2] = 2;
			result_shot = SRV_LOGIC_RESULT_HIT;
			c_t_m->count = c_t_m->count + 1;
		}
	}
	if (c_t_m->ship4_1[1][4] == shot->x
			&& c_t_m->ship4_1[2][4] == shot->y) {
		if (c_t_m->ship4_1[3][1] == 2
				&& c_t_m->ship4_1[3][2] == 2
				&& c_t_m->ship4_1[3][3] == 2) {
			c_t_m->ship4_1[3][1] = 3;
			c_t_m->ship4_1[3][2] = 3;
			c_t_m->ship4_1[3][3] = 3;
			c_t_m->ship4_1[3][4] = 3;
			result_shot = SRV_LOGIC_RESULT_KILL;
			c_t_m->count = c_t_m->count + 1;
		} else {
			c_t_m->ship4_1[3][4] = 2;
			result_shot = SRV_LOGIC_RESULT_HIT;
			c_t_m->count = c_t_m->count + 1;
		}
	}

	/*
	 *
	 * @when the counter becoms equel to 20 game over
	 * @won one who won the last shot
	 *
	 */

	if (c_t_m->count == 20) {
		result_shot = SRV_LOGIC_RESULT_END_GAME;
	} 

	/*
	 * @return the result of the shot
	 */
	return result_shot;

}

/*
 *
 * @function srv_logic_delmap clears the memory pointer to srv_logic_map
 *
 */

int srv_logic_delmap(struct srv_logic_map *c_t_m) {
	free(c_t_m);
	return 0;
}


