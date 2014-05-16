#ifndef QUIZDUELL_GAME_JSON
#define QUIZDUELL_GAME_JSON

#include "Quizduell_Structures.h"

Eina_Bool json_parse_current_game_info(const char *json);
Eina_Bool json_parse_login(const char *json);

#endif