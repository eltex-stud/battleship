enum main_event_types { GUI_NICK, GUI_SHOT, GUI_EXIT, NET_SHOT_RESULT,
                     NET_PLACEMENT, NET_ERROR, NET_START_GAME };
enum player_state { MY_TURN, ENEMY_TURN, WAITING_TURN };
enum player { ME, ENEMY };
typedef char placement[10][10];
typedef char map[10][10];