#include <stdio.h>
#include <json-c/json.h>
#include "Quizduell_Game_Json.h"
#include "Quizduell_Controller.h"
#include "Quizduell_Structures.h"

static Qd_Game_Info *_json_parse_game_info_game(Qd_Game_Info *_game_info, json_object *jobj);
static Qd_Player *_json_parse_player(json_object *jobj);

Qd_Game_Id json_parse_game_for_game_id(const char *json)
{
    json_object *jobj, *tmp = NULL;

    if (!(jobj = json_tokener_parse(json))
        || !(tmp = json_object_object_get(jobj, "game"))
        || !(tmp = json_object_object_get(tmp, "game_id")))
    {
        return 0;
    }

    return json_object_get_int64(tmp);
}

static Qd_Game_Info *_json_parse_game_info_game(Qd_Game_Info *_game_info, json_object *jobj)
{
    Qd_Game_Info *game_info = NULL;
    json_object *tmp = NULL, *o = NULL;
    array_list *arr = NULL;
    int i = 0, j = 0;

    if (_game_info)
    {
        game_info = _game_info;
    }
    else
    {
        game_info = calloc(1, sizeof(Qd_Game_Info));
    }

    if ((tmp = json_object_object_get(jobj, "your_answers"))) // array of ints
    {
        for (i = 0, arr = json_object_get_array(tmp);
            i < (sizeof(game_info->your_answers) / sizeof(game_info->your_answers[0][0])); 
            i++, o = NULL)
        {
            unsigned int rnd = i / 3;
            unsigned int game = i % 3;
            o = json_object_array_get_idx(tmp, i);
            game_info->your_answers[rnd][game] = o ? json_object_get_int(o) : QD_INVALID_VALUE;
        }
    }
    else
    {
        for (i = 0; i < NO_ROUNDS_PER_GAME; i++)
        {
            for (j = 0; j < NO_QUESTIONS_PER_ROUND; j++)
            {
                game_info->your_answers[i][j] = QD_INVALID_VALUE;            
            }
        }
    }

    if ((tmp = json_object_object_get(jobj, "state")))
    {
        game_info->state = json_object_get_int(tmp);
    }

    if ((tmp = json_object_object_get(jobj, "you_gave_up")))
    {
        game_info->you_gave_up = json_object_get_boolean(tmp);
    }
    else
    {
        game_info->you_gave_up = EINA_FALSE;
    }

    if ((tmp = json_object_object_get(jobj, "elapsed_min")))
    {
        game_info->elapsed_min = json_object_get_int(tmp);
    }

    if ((tmp = json_object_object_get(jobj, "rating_bonus")))
    {
        game_info->rating_bonus = json_object_get_int(tmp);
    }

    if ((tmp = json_object_object_get(jobj, "your_turn")))
    {
        game_info->your_turn = json_object_get_boolean(tmp);
    }

    game_info->game_id = json_object_get_int64(json_object_object_get(jobj, "game_id"));

    if ((tmp = json_object_object_get(jobj, "give_up_player_id")))
    {
        game_info->give_up_player_id = json_object_get_int64(tmp);
    }
    else
    {
        game_info->give_up_player_id = QD_USER_ID_UNDEFINED;
    }


    if ((tmp = json_object_object_get(jobj, "cat_choices"))) // array of ints
    {
        for (i = 0, arr = json_object_get_array(tmp), o = NULL; 
            i < (sizeof(game_info->cat_choices) / sizeof(game_info->cat_choices[0]));
            i++, o = NULL)
        {
            o = json_object_array_get_idx(tmp, i);
            game_info->cat_choices[i] = o ? json_object_get_int(o) : QD_INVALID_VALUE;
        }
    }
    else
    {
        for (i = 0; i < (sizeof(game_info->cat_choices) / sizeof(game_info->cat_choices[0])); i++)
        {
            game_info->cat_choices[i] = QD_INVALID_VALUE;            
        }
    }

    if ((tmp = json_object_object_get(jobj, "opponent_answers"))) // array of ints
    {
        for (i = 0, arr = json_object_get_array(tmp), o = NULL;
            i < (sizeof(game_info->opponent_answers) / sizeof(game_info->opponent_answers[0][0]));
            i++, o = NULL)
        {
            unsigned int rnd = i / 3;
            unsigned int game = i % 3;
            o = json_object_array_get_idx(tmp, i);
            game_info->opponent_answers[rnd][game] = o ? json_object_get_int(o) : QD_INVALID_VALUE;
        }        
    }
    else
    {
        for (i = 0; i < NO_ROUNDS_PER_GAME; i++)
        {
            for (j = 0; j < NO_QUESTIONS_PER_ROUND; j++)
            {
                game_info->opponent_answers[i][j] = QD_INVALID_VALUE;            
            }
        }
    }

    //TODO: actually copy messages
    game_info->messages = NULL;

    if ((tmp = json_object_object_get(jobj, "opponent")))
    {
        game_info->opponent = _json_parse_player(tmp);
    }

    return game_info;
}

static Qd_Player *_json_parse_player(json_object *jobj)
{
    Qd_Player *p = malloc(sizeof(Qd_Player));
    json_object *tmp = NULL;

    if (!(tmp = json_object_object_get(jobj, "user_id")))
    {
        return NULL;
    }
    p->user_id = atol(json_object_get_string(tmp));
    tmp = json_object_object_get(jobj, "name");
    if (!tmp)
    {
        return NULL;
    }
    p->name = eina_stringshare_add(json_object_get_string(tmp));

    return p;
}

Qd_Game_Info *json_parse_game_info_game(Qd_Game_Info *_game_info, const char *json)
{
    json_object *jobj = NULL;

    if (!(jobj = json_tokener_parse(json)))
    {
        return NULL;
    }

    return _json_parse_game_info_game(_game_info, jobj);
}

Eina_Bool json_parse_users_current_user_games(const char *json)
{
    json_object *user = NULL, *tmp = NULL;
    array_list *arr = NULL;
    int i = 0;
    int no_games = 0, no_friends = 0;
    Qd_Game_Info *game = NULL;
    Qd_Player *_friend = NULL;
    json_object *jobj = NULL;

    if (!(jobj = json_tokener_parse(json)))
    {
        return EINA_FALSE;
    }

    if (!(user = json_object_object_get(jobj, "user")))
    {
        return EINA_FALSE;
    }
    qd_player_free(player);
    player = NULL;
    if (!(player = _json_parse_player(user)))
    {
        return EINA_FALSE;
    }

    // parse friends
    EINA_LIST_FREE(friends, _friend)
    {
        qd_player_free(_friend);
    }
    friends = NULL;

    tmp = json_object_object_get(user, "friends"); // array of ints
    for (i = 0, arr = json_object_get_array(tmp), no_friends = json_object_array_length(tmp); i < no_friends; i++)
    {
        json_object *o = json_object_array_get_idx(tmp, i);
        _friend = _json_parse_player(o);
        friends = eina_list_append(friends, _friend);
    }

    // parse games
    EINA_LIST_FREE(games, game)
    {
        qd_game_info_free(game);
    }
    games = NULL;

    tmp = json_object_object_get(user, "games"); // array of ints
    for (i = 0, arr = json_object_get_array(tmp), no_games = json_object_array_length(tmp); i < no_games; i++)
    {
        json_object *o = json_object_array_get_idx(tmp, i);
        game = _json_parse_game_info_game(NULL, o);
        games = eina_list_append(games, game);
    }

    return EINA_TRUE;
}

Eina_Bool json_parse_games_upload_round_answers(Qd_Game_Info *game, const char *json)
{
    json_object *jobj, *game_obj;

    if ((jobj = json_tokener_parse(json)) && (game_obj = json_object_object_get(jobj, "game")) && _json_parse_game_info_game(game, game_obj))
    {
        return EINA_TRUE;
    }

    return EINA_FALSE;
}

Eina_Bool json_parse_login(const char *json)
{
    return json_parse_users_current_user_games(json);
}

Qd_Question *_json_parse_question(json_object *jobj)
{
    Qd_Question *q = malloc(sizeof(Qd_Question));
    json_object *tmp = NULL;

    #define GET_VAL(val) \
        if (!(tmp = json_object_object_get(jobj, val))) \
            return NULL
        GET_VAL("cat_id");
        q->cat_id = json_object_get_int(tmp);
        GET_VAL("cat_name");
        q->cat_name = eina_stringshare_add(json_object_get_string(tmp));
        GET_VAL("correct");
        q->correct = eina_stringshare_add(json_object_get_string(tmp));
        GET_VAL("q_id");
        q->q_id = json_object_get_int64(tmp);
        GET_VAL("question");
        q->question = eina_stringshare_add(json_object_get_string(tmp));
        GET_VAL("wrong1");
        q->wrong[0] = eina_stringshare_add(json_object_get_string(tmp));
        GET_VAL("wrong2");
        q->wrong[1] = eina_stringshare_add(json_object_get_string(tmp));
        GET_VAL("wrong3");
        q->wrong[2] = eina_stringshare_add(json_object_get_string(tmp));
    #undef GET_VAL

    return q;
}

Qd_Game_Info *json_parse_specific_game_info(Qd_Game_Info *game, const char *json)
{
    json_object *jobj = NULL;
    json_object *game_obj, *tmp = NULL;
    array_list *arr = NULL;
    int i = 0, no_questions = 0, rnd = 0, cat_choice = 0;

    if (!(jobj = json_tokener_parse(json)))
    {
        return NULL;
    }

    if ((tmp = json_object_object_get(jobj, "access")))
    {
        Eina_Bool access = json_object_get_boolean(tmp);
        if (!access)
        {
            // this means that either our cookie is dead or we didn't send one at all
            return NULL;
        }
    }

    if (!(game_obj = json_object_object_get(jobj, "game")))
    {
        return NULL;
    }
    game = _json_parse_game_info_game(game, game_obj);

    // parse questions
    tmp = json_object_object_get(game_obj, "questions"); // array of questions
    no_questions = json_object_array_length(tmp);
    arr = json_object_get_array(tmp);
    for (rnd = 0; rnd < NO_ROUNDS_PER_GAME; rnd++)
    {
        for (cat_choice = 0; cat_choice < NO_CAT_CHOICES; cat_choice++)
        {
            for (i = 0; i < NO_QUESTIONS_PER_ROUND; i++)
            {
                unsigned int idx = (rnd * NO_CAT_CHOICES * NO_QUESTIONS_PER_ROUND) + (cat_choice * NO_QUESTIONS_PER_ROUND) + i;
                json_object *o = json_object_array_get_idx(tmp, idx);
                game->questions[rnd][cat_choice][i] = _json_parse_question(o);
            }
        }
    }

    return game;
}

Qd_Server_Message *json_parse_server_message(const char *json)
{
    Qd_Server_Message *msg = calloc(1, sizeof(Qd_Server_Message));
    json_object *tmp = NULL;
    json_object *jobj = NULL;

    if (!(jobj = json_tokener_parse(json)))
    {
        goto _failed;
    }

    #define GET_VAL(val) \
        if (!(tmp = json_object_object_get(jobj, val))) \
            goto _failed;

        GET_VAL("popup_title");
        msg->title = eina_stringshare_add(json_object_get_string(tmp));
        GET_VAL("popup_mess");
        msg->msg = eina_stringshare_add(json_object_get_string(tmp));
    #undef GET_VAL

    return msg;

_failed:
    free(msg);
    return NULL;
}

Qd_Player *json_parse_users_find_user(const char *json)
{
    json_object *jobj, *player_jobj;

    if (!(jobj = json_tokener_parse(json)) || !(player_jobj = json_object_object_get(jobj, "u")))
    {
        return NULL;
    }

    return _json_parse_player(player_jobj);
}