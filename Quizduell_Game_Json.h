#ifndef QUIZDUELL_GAME_JSON
#define QUIZDUELL_GAME_JSON

#include "Quizduell_Structures.h"

Qd_Game_Info *json_parse_game(json_object *jobj);
void json_parse_player(json_object *jobj, Qd_Player *p);
void json_parse_current_game_info(json_object * jobj);

#endif