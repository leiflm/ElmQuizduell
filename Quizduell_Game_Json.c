#include <stdio.h>
#include <json/json.h>
#include "Quizduell_Game_Json.h"
#include "Quizduell_Controller.h"
#include "Quizduell_Structures.h"

Qd_Game_Info *json_parse_game(json_object *jobj)
{
    Qd_Game_Info *game_info = malloc(sizeof(Qd_Game_Info));
    json_object *tmp = NULL, *o = NULL;
    array_list *arr = NULL;
    int i = 0;
    int no_games = 0;

    tmp = json_object_object_get(jobj, "your_answers"); // array of ints
    for (i = 0, arr = json_object_get_array(tmp), no_games = json_object_array_length(tmp);
        i < (sizeof(game_info->your_answers) / sizeof(game_info->your_answers[0][0])); 
        i++, o = NULL)
    {
        unsigned int rnd = i / 3;
        unsigned int game = i % 3;
        o = json_object_array_get_idx(tmp, i);
        game_info->your_answers[rnd][game] = o ? json_object_get_int(o) : QD_INVALID_VALUE;
    }

    tmp = json_object_object_get(jobj, "state");
    game_info->state = json_object_get_int(tmp);

    tmp = json_object_object_get(jobj, "elapsed_min");
    game_info->elapsed_min = json_object_get_int(tmp);

    tmp = json_object_object_get(jobj, "rating_bonus");
    game_info->rating_bonus = json_object_get_int(tmp);

    tmp = json_object_object_get(jobj, "your_turn");
    game_info->your_turn = json_object_get_boolean(tmp);

    tmp = json_object_object_get(jobj, "game_id");
    game_info->game_id = json_object_get_int64(tmp);


    tmp = json_object_object_get(jobj, "cat_choices"); // array of ints
    for (i = 0, arr = json_object_get_array(tmp), no_games = json_object_array_length(tmp), o = NULL; 
        i < (sizeof(game_info->cat_choices) / sizeof(game_info->cat_choices[0]
            )); 
        i++, o = NULL)
    {
        if (i < no_games)
        {
            o = json_object_array_get_idx(tmp, i);
        }
        game_info->cat_choices[i] = o ? json_object_get_int(o) : QD_INVALID_VALUE;
    }

    tmp = json_object_object_get(jobj, "opponent_answers"); // array of ints
    for (i = 0, arr = json_object_get_array(tmp), no_games = json_object_array_length(tmp), o = NULL;
        i < (sizeof(game_info->opponent_answers) / sizeof(game_info->opponent_answers[0][0])); 
        i++, o = NULL)
    {
        unsigned int rnd = i / 3;
        unsigned int game = i % 3;
        if (i < no_games)
        {
            o = json_object_array_get_idx(tmp, i);
        }
        game_info->opponent_answers[rnd][game] = o ? json_object_get_int(o) : QD_INVALID_VALUE;
    }

    //TODO: actually copy messages
    game_info->messages = NULL;

    tmp = json_object_object_get(jobj, "opponent");
    json_parse_player(tmp, &(game_info->opponent));

    return game_info;
}

void json_parse_player(json_object *jobj, Qd_Player *p)
{
    json_object *tmp = NULL;

    tmp = json_object_object_get(jobj, "user_id");
    p->user_id = atol(json_object_get_string(tmp));
    tmp = json_object_object_get(jobj, "name");
    p->name = eina_stringshare_add(json_object_get_string(tmp));
}

void json_parse_current_game_info(json_object * jobj) {
    json_object *user = NULL, *tmp = NULL;
    array_list *arr = NULL;
    int i = 0;
    int no_games = 0;
    Qd_Game_Info *game = NULL;

    user = json_object_object_get(jobj, "user");
    json_parse_player(user, &player);

    // parse games
    tmp = json_object_object_get(user, "games"); // array of ints
    for (arr = json_object_get_array(tmp), no_games = json_object_array_length(tmp); i < no_games; i++)
    {
        json_object *o = json_object_array_get_idx(tmp, i);
        game = json_parse_game(o);
        games = eina_list_append(games, game);
    }
}