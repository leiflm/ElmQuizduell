#include <json-c/json.h>
#include <Eina.h>
#include <Ecore.h>
#include "Quizduell_Structures.h"
#include "Quizduell_Config.h"
#include "Quizduell_Crypto.h"
#include "Quizduell_Connection.h"
#include "Quizduell_Game_Json.h"
#include "Quizduell_View.h"
#include "Quizduell_Controller.h"

static void _qd_ctrl_data_free(void);
static Eina_Bool _qd_ctrl_users_login_completed_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event_info);
static Eina_Bool _qd_ctrl_users_current_user_games_completed_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event_info);
static Eina_Bool _qd_ctrl_games_create_game_completed_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event_info);
static Eina_Bool _qd_ctrl_games_give_up_completed_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event_info);
static Eina_Bool _qd_ctrl_games_specific_game_info_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event_info);
static Eina_Bool _qd_ctrl_games_upload_round_answers_completed_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event_info);
static Eina_Bool _qd_ctrl_users_find_user_completed_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event_info);
static Eina_Bool _qd_ctrl_users_add_friend_completed_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event_info);

Qd_Player *player = NULL;
Eina_List *games = NULL; // list of Qd_Game_Info*
Eina_List *friends = NULL; // list of Qd_Player*

static Eina_Stringshare *username = NULL;
static Eina_Stringshare *password = NULL;

static void _init_event_cbs(void)
{
    ecore_event_handler_add(QD_CON_USERS_LOGIN, _qd_ctrl_users_login_completed_cb, NULL);
    ecore_event_handler_add(QD_CON_USERS_CURRENT_USER_GAMES, _qd_ctrl_users_current_user_games_completed_cb, NULL);
    ecore_event_handler_add(QD_CON_GAMES_CREATE_GAME, _qd_ctrl_games_create_game_completed_cb, NULL);
    ecore_event_handler_add(QD_CON_GAMES_GIVE_UP, _qd_ctrl_games_give_up_completed_cb, NULL);
    ecore_event_handler_add(QD_CON_GAMES_SPECIFIC_GAME_INFO, _qd_ctrl_games_specific_game_info_cb, NULL);
    ecore_event_handler_add(QD_CON_GAMES_UPLOAD_ROUND_ANSWERS, _qd_ctrl_games_upload_round_answers_completed_cb, NULL);
    ecore_event_handler_add(QD_CON_USERS_FIND_USER, _qd_ctrl_users_find_user_completed_cb, NULL);
    ecore_event_handler_add(QD_CON_USERS_ADD_FRIEND, _qd_ctrl_users_add_friend_completed_cb, NULL);
}

Eina_Bool qd_ctrl_init(void)
{
    eina_init();
    ecore_init();
    _init_event_cbs();
    return EINA_TRUE;
}

void qd_ctrl_shutdown(void)
{
    _qd_ctrl_data_free();
    eina_stringshare_del(username), username = NULL;
    eina_stringshare_del(password), password = NULL;
    ecore_shutdown();
    eina_shutdown();
}

void qd_ctrl_run(void)
{
    if (qd_con_cookie_exists)
        qd_ctrl_users_current_user_games();
    ecore_main_loop_begin();
}

void qd_ctrl_users_create(Eina_Stringshare *name, Eina_Stringshare *pwd)
{
    char *_password_hash = NULL;
    printf("creating account for: %s, pwd: %s\n", name, pwd);

    //TODO: Add lock for login try
    username = eina_stringshare_add(name);
    password = eina_stringshare_add(pwd);
    _password_hash = qd_crypto_create_password_hash(password);

    Eina_Hash *hash = eina_hash_string_superfast_new((Eina_Free_Cb)eina_stringshare_del);
    eina_hash_add(hash, "name", eina_stringshare_add(username));
    eina_hash_add(hash, "pwd", eina_stringshare_add(_password_hash));
    qd_con_request_with_params(NULL, "users/create", hash, QD_CON_USERS_LOGIN, EINA_TRUE);
    eina_hash_free(hash);
    hash = NULL;
    free(_password_hash);
}

void qd_ctrl_users_login(Eina_Stringshare *name, Eina_Stringshare *pwd)
{
    char *_password_hash = NULL;
    printf("logging in: %s, pw: %s\n", name, pwd);

    //TODO: Add lock for login try
    username = eina_stringshare_add(name);
    password = eina_stringshare_add(pwd);
    _password_hash = qd_crypto_create_password_hash(password);

    Eina_Hash *hash = eina_hash_string_superfast_new((Eina_Free_Cb)eina_stringshare_del);
    eina_hash_add(hash, "name", eina_stringshare_add(username));
    eina_hash_add(hash, "pwd", eina_stringshare_add(_password_hash));
    qd_con_request_with_params(NULL, "users/login", hash, QD_CON_USERS_LOGIN, EINA_TRUE);
    eina_hash_free(hash);
    hash = NULL;
    free(_password_hash);
}

void qd_ctrl_users_current_user_games(void)
{
    qd_con_request_with_params(NULL, "users/current_user_games", NULL, QD_CON_USERS_CURRENT_USER_GAMES, EINA_TRUE);
}

void qd_ctrl_games_game_create(const Qd_User_Id uid)
{
    Eina_Hash *hash = eina_hash_string_superfast_new((Eina_Free_Cb)eina_stringshare_del);
    char buf[64];

    snprintf(buf, sizeof(buf), "%lu", uid);

    eina_hash_add(hash, "opponent_id", eina_stringshare_add(buf));
    qd_con_request_with_params(NULL, "games/create_game", hash, QD_CON_GAMES_CREATE_GAME, EINA_TRUE);

    eina_hash_free(hash);
}

void qd_ctrl_games_give_up(Qd_Game_Info *game_info)
{
    Eina_Hash *hash = eina_hash_string_superfast_new((Eina_Free_Cb)eina_stringshare_del);
    char buf[64];

    snprintf(buf, sizeof(buf), "%lu", game_info->game_id);

    eina_hash_add(hash, "game_id", eina_stringshare_add(buf));
    qd_con_request_with_params(NULL, "games/give_up", hash, QD_CON_GAMES_GIVE_UP, EINA_TRUE);

    eina_hash_free(hash);
}

void qd_ctrl_game_details(Qd_Game_Info *game)
{
    Eina_Strbuf *url = eina_strbuf_new();

    eina_strbuf_append_printf(url, "games/%li", game->game_id);
    qd_con_request_with_params(game, eina_strbuf_string_get(url), NULL, QD_CON_GAMES_SPECIFIC_GAME_INFO, EINA_FALSE);

    eina_strbuf_free(url);
}
/*
void qd_ctrl_game_details(Qd_Game_Info *game)
{
    qd_view_game_stat_page_show(game);
}

*/
void qd_ctrl_games_list_update(void)
{
    Eina_List *l;
    Qd_Game_Info *game;
    qd_view_games_list_clear();
    EINA_LIST_FOREACH(games, l, game)
    {
        switch (game->state)
        {
            case QD_GAME_STATE_PLAYING:
            {
                if (game->your_turn)
                {
                    qd_view_games_list_active_item_add(game);
                }
                else
                {
                    qd_view_games_list_inactive_item_add(game);
                }
                break;
            }
            case QD_GAME_STATE_NONE:
            {
                qd_view_new_game_challenge_popup(game);
                break;
            }
            default:
                qd_view_games_list_done_item_add(game);
        }
    }
}

void qd_ctrl_answers_send(Qd_Game_Info *game)
{
    char buf[256];
    unsigned int category = QD_INVALID_VALUE;
    Eina_Hash *hash = eina_hash_string_superfast_new((Eina_Free_Cb)eina_stringshare_del);
    Eina_Strbuf *strbuf = eina_strbuf_new();
    int i = 0;

    printf("Sending answers for game %lu\n", game->game_id);

    for (i = 0; i < NO_ROUNDS_PER_GAME; i++)
    {
        if (game->cat_choices[i] == QD_INVALID_VALUE)
        {
            break;
        }
        category = game->cat_choices[i];
    }
    snprintf(buf, sizeof(buf), "%i", category);
    eina_hash_add(hash, "cat_choice", eina_stringshare_add(buf));

    snprintf(buf, sizeof(buf), "%lu", game->game_id);
    eina_hash_add(hash, "game_id", eina_stringshare_add(buf));

    eina_strbuf_append(strbuf, "[");
    for (i = 0; i < NO_ROUNDS_PER_GAME; i++)
    {
        int j = 0;
        for (j = 0; j < NO_QUESTIONS_PER_ROUND; j++)
        {
            if (game->cat_choices[i] == QD_INVALID_VALUE)
            {
                break;
            }
            if ((i > 0) || (j > 0))
            {
                eina_strbuf_append(strbuf, ",");
            }
            eina_strbuf_append_printf(strbuf, "%i", game->your_answers[i][j]);
        }
    }
    eina_strbuf_append(strbuf, "]");
    eina_hash_add(hash, "answers", eina_stringshare_add(eina_strbuf_string_get(strbuf)));
    eina_strbuf_free(strbuf);

    qd_con_request_with_params(game, "games/upload_round_answers", hash, QD_CON_GAMES_UPLOAD_ROUND_ANSWERS, EINA_TRUE);
    eina_hash_free(hash);
    hash = NULL;
}

void qd_ctrl_users_find_user(Eina_Stringshare *username)
{
    Eina_Hash *hash = NULL;

    if (!username)
    {
        printf("Missing username to search for!\n");
        return;
    }

    hash = eina_hash_string_superfast_new((Eina_Free_Cb)eina_stringshare_del);
    username = eina_stringshare_add(username);

    eina_hash_add(hash, "opponent_name", username);

    qd_con_request_with_params(NULL, "users/find_user", hash, QD_CON_USERS_FIND_USER, EINA_TRUE);
    eina_hash_free(hash);
}

void qd_ctrl_users_add_friend(Qd_User_Id uid)
{
    Eina_Hash *hash = eina_hash_string_superfast_new((Eina_Free_Cb)eina_stringshare_del);
    char buf[64];

    snprintf(buf, sizeof(buf), "%lu", uid);

    eina_hash_add(hash, "friend_id", eina_stringshare_add(buf));

    qd_con_request_with_params(NULL, "users/add_friend", hash, QD_CON_USERS_ADD_FRIEND, EINA_TRUE);
    eina_hash_free(hash);
}

// static void _qd_ctrl_user_login_success()
// {
//     qd_config.username = username;
//     qd_config.password = password;
// }

// static void _qd_ctrl_user_login_failure()
// {
//     eina_stringshare_del(username), username = NULL;
//     eina_stringshare_del(password), password = NULL;
// }

static Eina_Bool _qd_ctrl_users_login_completed_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event_info)
{
    Qd_Con_Request *rqst = event_info;
    const char *server_response = eina_strbuf_string_get(rqst->buffer);

    printf("Login completed\n");
    printf("%s\n", server_response);

    if (!json_parse_login(server_response))
    {
        return EINA_TRUE;
    }

    // on login set new name
    // FIXME: use stringshare all over the place!
    qd_ctrl_games_list_update();
    qd_view_user_name_set(username);
    qd_view_games_list_page_show();

    return EINA_TRUE;
}

static Eina_Bool _qd_ctrl_users_current_user_games_completed_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event_info)
{
    Qd_Con_Request *rqst = event_info;
    const char *server_response = eina_strbuf_string_get(rqst->buffer);

    printf("Fetched current_user_games completed\n");
    printf("%s\n", server_response);

    if (json_parse_users_current_user_games(server_response))
    {
        qd_ctrl_games_list_update();
        qd_view_user_name_set(player->name);
    }

    return EINA_TRUE;
}

static Eina_Bool _qd_ctrl_games_create_game_completed_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event_info)
{
    Qd_Con_Request* rqst = event_info;
    const char *server_response = eina_strbuf_string_get(rqst->buffer);
    Qd_Game_Info *game = (Qd_Game_Info*)rqst->game_info;

    printf("Received specific game info\n");
    printf("%s\n", server_response);

    if ((game = json_parse_specific_game_info(game, server_response)))
    {
        qd_view_game_stat_page_show(game);
    }

    return EINA_TRUE;
}

static Eina_Bool _qd_ctrl_games_give_up_completed_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event_info)
{
    Qd_Con_Request* rqst = event_info;
    const char *server_response = eina_strbuf_string_get(rqst->buffer);
    Qd_Game_Info *game = (Qd_Game_Info*)rqst->game_info;

    printf("Received specific game info\n");
    printf("%s\n", server_response);

    json_parse_game_info_game(game, server_response);

    //qd_view_game_stat_page_show(game);
    qd_ctrl_games_list_update();
    qd_view_games_list_page_show();
    return EINA_TRUE;
}

static Eina_Bool _qd_ctrl_games_specific_game_info_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event_info)
{
    Qd_Con_Request* rqst = event_info;
    const char *server_response = eina_strbuf_string_get(rqst->buffer);
    Qd_Game_Info *game = (Qd_Game_Info*)rqst->game_info;

    printf("Received specific game info\n");
    printf("%s\n", eina_strbuf_string_get(rqst->buffer));

    if (json_parse_specific_game_info(game, server_response))
    {
        qd_view_game_stat_page_show(game);
    }

    return EINA_TRUE;
}

static Eina_Bool _qd_ctrl_games_upload_round_answers_completed_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event_info)
{
    Qd_Con_Request *rqst = event_info;
    const char *server_response = eina_strbuf_string_get(rqst->buffer);
    Qd_Game_Info *game = NULL;
    Qd_Game_Id game_id = json_parse_game_for_game_id(server_response);
    Eina_List *iter;

    printf("Values were uploaded!\n");

    EINA_LIST_FOREACH(games, iter, game)
    {
        if (game->game_id == game_id)
        {
            json_parse_games_upload_round_answers(game, server_response);
            printf("Game Info structure was updated in-place too!\n");
            break;
        }
    }

    return EINA_TRUE;
}

static Eina_Bool _qd_ctrl_users_find_user_completed_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event_info)
{
    Qd_Con_Request* rqst = event_info;
    const char *server_response = eina_strbuf_string_get(rqst->buffer);
    Qd_Player *player = json_parse_users_find_user(server_response);

    printf("Player searched!\n");

    if (player)
    {
        printf("Found player: %s\n", player->name);
    }
    else
    {
        printf("Found none though!\n");
    }
    qd_view_search_player_completed_cb(player);

    return EINA_TRUE;
}

static Eina_Bool _qd_ctrl_users_add_friend_completed_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event_info)
{
    return EINA_TRUE;
}

static void _qd_ctrl_data_free(void)
{
    Qd_Game_Info *game = NULL;
    Qd_Player *_friend = NULL;

    EINA_LIST_FREE(friends, _friend)
    {
        qd_player_free(_friend);
    }
    friends = NULL;

    EINA_LIST_FREE(games, game)
    {
        int i = 0;
        printf("opponent: %s\n", game->opponent->name);
        printf("game_id: %li\n", game->game_id);
        for (i = 0; i < 18; ++i)
        {
            int rnd = i / 3;
            int j = i % 3;
            printf("opponent answer: %i\n", game->opponent_answers[rnd][j]);
        }
        qd_game_info_free(game);
    }
    qd_player_free(player);
}
