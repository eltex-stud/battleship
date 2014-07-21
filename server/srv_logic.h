/* vim: set noexpandtab tabstop=4 shiftwidth=4 smartindent: */
#ifndef BATTLE_SERVER_LOGIC_H_
#define BATTLE_SERVER_LOGIC_H_

/*
 *
 * @create a shot list of result
 *
 */ 

enum srv_logic_shot_result {
	SRV_LOGIC_RESULT_HIT = 2,
	SRV_LOGIC_RESULT_KILL,
	SRV_LOGIC_RESULT_MISS,
	SRV_LOGIC_RESULT_END_GAME
};

/*
 *
 * @struct srv_logic_shot stires the coordinates of shots
 *
 */

struct srv_logic_shot {
	int x;
	int y;
};

/*
 *
 * @function placement_to_map creatrs and fills structure srv_logic_map
 * @placement[10][10] a binary map
 *
 */

struct srv_logic_map *placement_to_map(int placement[10][10]);

/*
 *
 * @function srv_logic_delmap clears the memory pointer to srv_logic_map
 *
 */

int srv_logic_delmap(struct srv_logic_map *c_t_m);

/*
 *
 * @function make_shot takes the shot structure coordinates 
 * and structure coordinates of ships
 * @function make_shot returns a shot and games
 *
 */

enum srv_logic_shot_result make_shot(struct srv_logic_map *c_t_m,
		struct srv_logic_shot *shot);

/*
 *
 * @function srv_logic_delmap clears the memory pointer to srv_logic_map
 *
 */

int srv_logic_delmap(struct srv_logic_map *c_t_m);

#endif // BATTLE_SERVER_LOGIC_H_
