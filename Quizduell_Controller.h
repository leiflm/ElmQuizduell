#ifndef __QUIZDUELL_CONTROLLER_H__
#define __QUIZDUELL_CONTROLLER_H__

#include <Quizduell_Structures.h>

extern Qd_Player *player;
extern Eina_List *games; // list of Qd_Game_Info*

Eina_Bool qd_ctrl_init(void);
void qd_ctrl_shutdown(void);
void qd_ctrl_run(void);

void qd_ctrl_user_login(char *name, char* pw);

// find user online by string
void qd_ctrl_users_find_user(Eina_Stringshare *username);

// Add friend by her q_id
void qd_ctrl_users_add_friend(Qd_User_Id uid);

// Reload the user state (games and settings)
void qd_ctrl_users_current_user_games(void);

// reload if there are new pending games active
void qd_ctrl_games_list_reload(void);

// start a new game against a random player
void qd_ctrl_game_new_random_player(void);

// round finished and updated answers in game struct, now send
void qd_ctrl_answers_send(Qd_Game_Info *game);

// update view lists
void qd_ctrl_games_list_update(void);

// Get data/details for specified game and show stat page afterwards
void qd_ctrl_game_details(Qd_Game_Info *game);

// list of Qd_Game_Info
Eina_List *Games;
// list of Qd_Player
Eina_List *Friends;

#endif
