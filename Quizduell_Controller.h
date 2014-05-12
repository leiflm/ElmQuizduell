#ifndef __QUIZDUELL_CONTROLLER_H__
#define __QUIZDUELL_CONTROLLER_H__

#include <Quizduell_Structures.h>

extern Qd_Player player;
extern Eina_List *games; // list of Qd_Game_Info*

Eina_Bool qd_ctrl_init(void);
void qd_ctrl_shutdown(void);
void qd_ctrl_run(void);

void qd_ctrl_user_login(char *name, char* pw);

// reload if there are new pending games active
void qd_ctrl_games_list_reload(void);

// start a new game against a random player
void qd_ctrl_game_random_player_new(void);

// search for player
void qd_ctrl_game_player_search(char *name);

// append to 

// list of Qd_Game_Info
Eina_List *Games;
// list of Qd_Player
Eina_List *Friends;

#endif
