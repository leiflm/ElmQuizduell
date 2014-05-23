#ifndef QUIZDUELL_GAME_JSON
#define QUIZDUELL_GAME_JSON

#include "Quizduell_Structures.h"

Eina_Bool json_parse_current_game_info(const char *json);
Eina_Bool json_parse_specific_game_info(Qd_Game_Info *game_info, const char *json);
Eina_Bool json_parse_login(const char *json);
Qd_Server_Message *json_parse_server_message(const char *json);
Qd_Player *json_parse_users_find_user(const char *json);

#endif