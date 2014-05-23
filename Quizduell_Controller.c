#include <json/json.h>
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
static Eina_Bool _qd_ctrl_games_specific_game_info_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event_info);
static Eina_Bool _qd_ctrl_games_upload_round_answers_completed_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event_info);
static Eina_Bool _qd_ctrl_users_find_user_completed_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event_info);
static Eina_Bool _qd_ctrl_users_add_friend_completed_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event_info);

Qd_Player *player = NULL;
Eina_List *games = NULL; // list of Qd_Game_Info*

static Eina_Stringshare *_tmp_username = NULL;
static Eina_Stringshare *_tmp_password = NULL;

static void _init_event_cbs(void)
{
    ecore_event_handler_add(QD_CON_USERS_LOGIN, _qd_ctrl_users_login_completed_cb, NULL);
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
    eina_stringshare_del(_tmp_username), _tmp_username = NULL;
    eina_stringshare_del(_tmp_password), _tmp_password = NULL;
    ecore_shutdown();
    eina_shutdown();
}

void qd_ctrl_run(void)
{
    ecore_main_loop_begin();
}

void qd_ctrl_user_login(char *name, char *pw)
{
    char *_password_hash = NULL;
    printf("logging in: %s, pw: %s\n", name, pw);

    //TODO: Add lock for login try
    _tmp_username = eina_stringshare_add(name);
    _tmp_password = eina_stringshare_add(pw);
    _password_hash = qd_crypto_create_password_hash(_tmp_password);

    Eina_Hash *hash = eina_hash_string_superfast_new((Eina_Free_Cb)eina_stringshare_del);
    eina_hash_add(hash, "name", eina_stringshare_add(_tmp_username));
    eina_hash_add(hash, "pwd", eina_stringshare_add(_password_hash));
    qd_con_request_with_params(NULL, "users/login", hash, QD_CON_USERS_LOGIN, EINA_TRUE);
    eina_hash_free(hash);
    hash = NULL;
    free(_password_hash);
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
        if (game->state == QD_GAME_STATE_PLAYING)
        {
            if (game->your_turn)
            {
                qd_view_games_list_active_item_add(game);
            }
            else
            {
                qd_view_games_list_inactive_item_add(game);
            }
        }
        else
            qd_view_games_list_done_item_add(game);
    }
}

void qd_ctrl_answers_send(Qd_Game_Info *game)
{
    char buf[256];
    unsigned int category = QD_INVALID_VALUE;
    Eina_Hash *hash = eina_hash_string_superfast_new((Eina_Free_Cb)eina_stringshare_del);
    Eina_Strbuf *strbuf = eina_strbuf_new();

    printf("Sending answers for game %lu\n", game->game_id);

    for (int i = 0; i < NO_ROUNDS_PER_GAME; i++)
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

    for (int i = 0; i < NO_ROUNDS_PER_GAME; i++)
    {
        for (int j = 0; j < NO_QUESTIONS_PER_ROUND; j++)
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
    eina_hash_add(hash, "game_id", eina_stringshare_add(eina_strbuf_string_get(strbuf)));
    eina_strbuf_free(strbuf);

    qd_con_request_with_params(game, "games/upload_round_answers", hash, QD_CON_GAMES_UPLOAD_ROUND_ANSWERS, EINA_TRUE);
    eina_hash_free(hash);
    hash = NULL;
}

Qd_Game_Info *qd_view_test_make_game(void)
{
   int i,j,k;
   Qd_Game_Info *game = calloc(1, sizeof(Qd_Game_Info));
   game->opponent = calloc(1, sizeof(Qd_Player));
   game->opponent->name = eina_stringshare_add("Test Player");
   game->state = QD_GAME_STATE_PLAYING;
   game->your_turn = EINA_TRUE;
   for (i = 0; i < NO_ROUNDS_PER_GAME; i++)
   {
        for (j = 0; j < NO_CAT_CHOICES; j++)
        {
             game->opponent_answers[i][j] = QD_INVALID_VALUE;
             game->your_answers[i][j] = QD_INVALID_VALUE;
             for (k = 0; k < NO_QUESTIONS_PER_ROUND; k++)
             {
                 game->questions[i][j][k] = calloc(1, sizeof(Qd_Question));
             }
        }
        game->cat_choices[i] = QD_INVALID_VALUE;
   }
   game->cat_choices[0] = 0;
   game->questions[0][0][0]->cat_name = eina_stringshare_add("Wunder der Technik");
   game->questions[0][0][0]->correct = eina_stringshare_add("Google");
   game->questions[0][0][0]->wrong[0] = eina_stringshare_add("MS");
   game->questions[0][0][0]->wrong[1] = eina_stringshare_add("Apple");
   game->questions[0][0][0]->wrong[2] = eina_stringshare_add("Amazon");
   game->questions[0][0][0]->question = eina_stringshare_add("What the search?");
   game->questions[0][0][1]->cat_name = eina_stringshare_add("Wunder der Technik");
   game->questions[0][0][1]->correct = eina_stringshare_add("Not Google");
   game->questions[0][0][1]->wrong[0] = eina_stringshare_add("MS");
   game->questions[0][0][1]->wrong[1] = eina_stringshare_add("ebuy");
   game->questions[0][0][1]->wrong[2] = eina_stringshare_add("Amazon");
   game->questions[0][0][1]->question = eina_stringshare_add("What the buy?");
   game->opponent_answers[0][0] = 0;
   game->opponent_answers[0][1] = 1;
   game->opponent_answers[0][2] = 0;
   game->your_answers[0][0] = 0;
   game->your_answers[0][1] = 3;
   game->your_answers[0][2] = 2;
   return game;
}

void qd_ctrl_game_new_random_player(void)
{
    Qd_Game_Info *game = qd_view_test_make_game();
    printf("Random player\n");
    games = eina_list_append(games, game);
    qd_view_games_list_page_show();
}

void qd_ctrl_users_find_user(Eina_Stringshare *username)
{
    Eina_Hash *hash = eina_hash_string_superfast_new((Eina_Free_Cb)eina_stringshare_del);
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
//     qd_config.username = _tmp_username;
//     qd_config.password = _tmp_password;
// }

// static void _qd_ctrl_user_login_failure()
// {
//     eina_stringshare_del(_tmp_username), _tmp_username = NULL;
//     eina_stringshare_del(_tmp_password), _tmp_password = NULL;
// }

static Eina_Bool _qd_ctrl_users_login_completed_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event_info)
{
    Eina_Strbuf *bytes = event_info;
    const char *server_response = eina_strbuf_string_get(bytes);

    printf("Login completed\n");
    printf("%s\n", eina_strbuf_string_get(bytes));

    if (!json_parse_login(server_response))
    {
        Qd_Server_Message *msg = json_parse_server_message(server_response);

        if (msg)
        {
            qd_view_info_message_show(msg->title, msg->msg);
            qd_server_message_free(msg);
        }
        else
        {
            qd_view_info_message_show("Ooops", "Login failed!");
        }
        return EINA_TRUE;
    }

    // on login set new name
    // FIXME: use stringshare all over the place!
    qd_view_user_name_set(strdup(_tmp_username));

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
    else
    {
        qd_view_info_message_show("Invalid session", "Please relogin or restart the application!");
    }

    return EINA_TRUE;
}

static Eina_Bool _qd_ctrl_games_upload_round_answers_completed_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event_info)
{
    // Eina_Strbuf *bytes = event_info;
    // const char *server_response = eina_strbuf_string_get(bytes);
    // Qd_Game_Info *game = NULL;

    printf("Values were uploaded!\n");
    // printf("%s\n", eina_strbuf_string_get(bytes));

    // game = json_parse_specific_game_info(server_response);

    // if (game)
    // {
    //     qd_view_game_stat_page_show(game);
    // }
    // else
    // {
    //     qd_view_info_message_show("Invalid session", "Please relogin or restart the application!");
    // }

    // eina_strbuf_free(bytes);

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

    return EINA_TRUE;
}

static Eina_Bool _qd_ctrl_users_add_friend_completed_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event_info)
{
    Qd_Con_Request* rqst = event_info;
    const char *server_response = eina_strbuf_string_get(rqst->buffer);
    Qd_Server_Message *msg = json_parse_server_message(server_response);

    if (msg)
    {
        qd_view_info_message_show(msg->title, msg->msg);
        qd_server_message_free(msg);
    }
    else
    {
        printf("Could not add friend!\n");
    }

    return EINA_TRUE;
}

static void _qd_ctrl_data_free(void)
{
    Qd_Game_Info *game = NULL;

    EINA_LIST_FREE(games, game)
    {
        printf("opponent: %s\n", game->opponent->name);
        printf("game_id: %li\n", game->game_id);
        for (int i = 0; i < 18; ++i)
        {
            int rnd = i / 3;
            int j = i % 3;
            printf("opponent answer: %i\n", game->opponent_answers[rnd][j]);
        }
        qd_game_info_free(game);
    }
    qd_player_free(player);
}
