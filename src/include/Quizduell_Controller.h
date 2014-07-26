#ifndef __QUIZDUELL_CONTROLLER_H__
#define __QUIZDUELL_CONTROLLER_H__

#include <Quizduell_Structures.h>

extern Qd_Player *player;
extern Eina_List *games; // list of Qd_Game_Info*
extern Eina_List *friends; // list of Qd_Player*

Eina_Bool qd_ctrl_init(void);
void qd_ctrl_shutdown(void);
void qd_ctrl_run(void);

// create an account with given username and password
void qd_ctrl_users_create(Eina_Stringshare *name, Eina_Stringshare *pwd);

void qd_ctrl_users_login(Eina_Stringshare *name, Eina_Stringshare *pwd);

// find user online by string
void qd_ctrl_users_find_user(Eina_Stringshare *username);

// Add friend by her q_id
void qd_ctrl_users_add_friend(Qd_User_Id uid);

// Reload the user state (games and settings)
void qd_ctrl_users_current_user_games(void);

// start a new game against a specific player
void qd_ctrl_games_game_create(const Qd_User_Id uid);

// give up a game
void qd_ctrl_games_give_up(Qd_Game_Info *game_info);

// start a new game against a random player
void qd_ctrl_game_new_random_player(void);

// round finished and updated answers in game struct, now send
void qd_ctrl_answers_send(Qd_Game_Info *game);

// update view lists
void qd_ctrl_games_list_update(void);

// Get data/details for specified game and show stat page afterwards
void qd_ctrl_game_details(Qd_Game_Info *game);

#endif
