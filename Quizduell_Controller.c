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

Qd_Player player;
Eina_List *games; // list of Qd_Game_Info*

static Eina_Stringshare *_tmp_username = NULL;
static Eina_Stringshare *_tmp_password = NULL;

static void _init_event_cbs(void)
{
    ecore_event_handler_add(QD_CON_USERS_LOGIN, _qd_ctrl_users_login_completed_cb, NULL);
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
    qd_con_request_with_params("users/login", hash, QD_CON_USERS_LOGIN, EINA_TRUE);
    eina_hash_free(hash);
    hash = NULL;
    free(_password_hash);
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

    json_parse_current_game_info(server_response);

    // on login set new name
    // FIXME: use stringshare all over the place!
    qd_view_user_name_set(strdup(_tmp_username));

    eina_strbuf_free(bytes);

    return EINA_TRUE;
}

static void _qd_ctrl_data_free(void)
{
    Qd_Game_Info *game = NULL;

    EINA_LIST_FREE(games, game)
    {
        printf("opponent: %s\n", game->opponent.name);
        printf("game_id: %li\n", game->game_id);
        for (int i = 0; i < 18; ++i)
        {
            int rnd = i / 3;
            int j = i % 3;
            printf("opponent answer: %i\n", game->opponent_answers[rnd][j]);
        }
        qd_game_info_free(game);
    }
    qd_player_free(&player);
}