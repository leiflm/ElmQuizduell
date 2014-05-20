#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <bsd/string.h>
#include <time.h>
#include <Eina.h>
#include <Ecore_Con.h>
#include "Quizduell_Config.h"
#include "Quizduell_Crypto.h"
#include "Quizduell_Connection.h"

struct _Qd_Con_Request
{
    Eina_Strbuf *buffer;
    int type;
};
typedef struct _Qd_Con_Request Qd_Con_Request;

static const char URL_REQUEST_CONTENT_TYPE[] = "application/x-www-form-urlencoded";

static Eina_Bool _qd_con_url_request_data_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event_info);
static Eina_Bool _qd_con_url_request_completed_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event_info);

int QD_CON_USERS_CREATE = 0;
int QD_CON_USERS_UPDATE = 0;
int QD_CON_USERS_LOGIN = 0;
int QD_CON_USERS_LOGOUT = 0;
int QD_CON_USERS_CURRENT_GAME_INFO = 0;
int QD_CON_GAMES_SPECIFIC_GAME_INFO = 0;
int QD_CON_GAMES_UPLOAD_ROUND_ANSWERS = 0;

static Qd_Con_Request *_qd_con_request_new(int type)
{
    Qd_Con_Request *rqst = malloc(sizeof(Qd_Con_Request));
    rqst->type = type;
    rqst->buffer = eina_strbuf_new();
    return rqst;
}

static void _qd_con_request_free_cb(void *user_data, void *func_data)
{
    free(func_data);
}

static void _init_events(void)
{
    #define EV_INIT(e) e = ecore_event_type_new()
        EV_INIT(QD_CON_USERS_CREATE);
        EV_INIT(QD_CON_USERS_UPDATE);
        EV_INIT(QD_CON_USERS_LOGIN);
        EV_INIT(QD_CON_USERS_LOGOUT);
        EV_INIT(QD_CON_USERS_CURRENT_GAME_INFO);
        EV_INIT(QD_CON_GAMES_SPECIFIC_GAME_INFO);
        EV_INIT(QD_CON_GAMES_UPLOAD_ROUND_ANSWERS);
    #undef EV_INIT
}

Eina_Bool qd_con_init(void)
{
    ecore_init();
    ecore_con_init();
	ecore_con_url_init();

    _init_events();
    ecore_event_handler_add(ECORE_CON_EVENT_URL_DATA, _qd_con_url_request_data_cb, NULL);
    ecore_event_handler_add(ECORE_CON_EVENT_URL_COMPLETE, _qd_con_url_request_completed_cb, NULL);

    return EINA_TRUE;
}

void qd_con_shutdown(void)
{
	ecore_con_url_shutdown();
    ecore_con_shutdown();
    ecore_shutdown();
}

static Eina_List *_get_sorted_list_for_params(const Eina_Hash *params_hashlist)
{
    Eina_List *sorted_list = NULL;
    Eina_Iterator *iter = NULL;
    const char *val = NULL;

    iter = eina_hash_iterator_data_new(params_hashlist);
    EINA_ITERATOR_FOREACH(iter, val)
    {
        sorted_list = eina_list_sorted_insert(sorted_list, (Eina_Compare_Cb)strcmp, val);
    }
    eina_iterator_free(iter);

    return sorted_list;
}

static Eina_Stringshare *_create_date_str(void)
{
    char date_str[64];
    time_t t;
    struct tm *tmp;

    t = time(NULL);
    tmp = localtime(&t);
    strftime(date_str, sizeof(date_str), "%F %T", tmp);

    return eina_stringshare_add(date_str);
}

static Eina_Stringshare *_create_hmac_with_params(const Eina_Strbuf *url_buf, const Eina_Hash *params_hashlist, Eina_Stringshare *date_str)
{
    Eina_Strbuf *hmac_input_buf = eina_strbuf_new();
    Eina_List *sorted_list = NULL;
    char *val;
    char *hmac = NULL;

    eina_strbuf_append_buffer(hmac_input_buf, url_buf);

    // append time string
    eina_strbuf_append(hmac_input_buf, date_str);

    // append all parameters, sorted
    sorted_list = _get_sorted_list_for_params(params_hashlist);
    EINA_LIST_FREE(sorted_list, val)
    {
        eina_strbuf_append(hmac_input_buf, val);
    }
    sorted_list = NULL;

    printf("Input for HMAC: \"%s\"\n", eina_strbuf_string_get(hmac_input_buf));
    hmac = qd_crypto_create_hmac(eina_strbuf_string_get(hmac_input_buf));
    eina_strbuf_free(hmac_input_buf);
    hmac_input_buf = NULL;

    return eina_stringshare_add(hmac);
}

Eina_Bool qd_con_request_with_params(const char *api_url, const Eina_Hash *params_hashlist, int type, Eina_Bool use_post)
{
    Ecore_Con_Url *url_con = NULL;
    Eina_Strbuf *url_buf = eina_strbuf_new();
    Eina_Strbuf *request_payload = eina_strbuf_new();
    Eina_Stringshare *date_str = NULL, *hmac = NULL;
    Eina_Iterator *it = NULL;
    Eina_Hash_Tuple *tuple = NULL;
    Qd_Con_Request *rqst = NULL;

    if (!(url_con = ecore_con_url_new(qd_config.decrypted_base_url)))
        return EINA_FALSE;

    //FIXME: Disable debugging
    ecore_con_url_verbose_set(url_con, EINA_TRUE);

    //FIXME: Import QD's actual certificate
    ecore_con_url_ssl_verify_peer_set(url_con, EINA_FALSE);

    eina_strbuf_append(url_buf, qd_config.decrypted_base_url);
    eina_strbuf_append(url_buf, api_url);

    // create date str
    date_str = _create_date_str();

    hmac = _create_hmac_with_params(url_buf, params_hashlist, date_str);
    printf("\tHMAC: \"%s\"\n", hmac);
    ecore_con_url_additional_header_add(url_con, "Authorization", hmac);

    it = eina_hash_iterator_tuple_new(params_hashlist);
    while (eina_iterator_next(it, (void**)&tuple))
    {
        const char *key = tuple->key;
        const char *value = tuple->data;
        eina_strbuf_append_printf(request_payload, "&%s=%s", key, value);
    }
    eina_iterator_free(it); // Always free the iterator after its use
    tuple = NULL;
    it = NULL;

    // set some agent
    ecore_con_url_additional_header_add(url_con, "User-Agent", qd_config.user_agent);
    // set device type
    ecore_con_url_additional_header_add(url_con, "dt", "a");

    // set clientdate for the server to validate hash
    ecore_con_url_additional_header_add(url_con, "clientdate", date_str);

    // use type in callback to select actual callback
    rqst = _qd_con_request_new(type);
    ecore_con_url_data_set(url_con, rqst);

    if (use_post)
    {
        ecore_con_url_url_set(url_con, eina_strbuf_string_get(url_buf));
        eina_strbuf_replace_first(request_payload, "&", "");
        ecore_con_url_post(url_con, eina_strbuf_string_get(request_payload), eina_strbuf_length_get(request_payload), URL_REQUEST_CONTENT_TYPE);
        printf("Using URL \"%s\" for POST request with data \"%s\"\n", ecore_con_url_url_get(url_con), eina_strbuf_string_get(request_payload));
    }
    else
    {
        //use GET
        eina_strbuf_append_buffer(url_buf, request_payload);
        ecore_con_url_url_set(url_con, eina_strbuf_string_get(url_buf));
        ecore_con_url_get(url_con);
    }

    eina_stringshare_del(date_str), date_str = NULL;
    eina_stringshare_del(hmac), hmac = NULL;
    eina_strbuf_free(url_buf), url_buf = NULL;
    eina_strbuf_free(request_payload), request_payload = NULL;
    
    return EINA_TRUE;
}

static Eina_Bool _qd_con_url_request_data_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event_info)
{
    Ecore_Con_Event_Url_Data *url_data = event_info;
    unsigned int i = 0;

    Qd_Con_Request *rqst = ecore_con_url_data_get(url_data->url_con);
    for (i = 0; i < url_data->size; i++)
    {
        eina_strbuf_append_char(rqst->buffer, url_data->data[i]);
    }
    return EINA_TRUE;
}

static Eina_Bool _qd_con_url_request_completed_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event_info)
{
    Ecore_Con_Event_Url_Complete *url_complete = event_info;
    Qd_Con_Request *rqst = ecore_con_url_data_get(url_complete->url_con);
    ecore_event_add(rqst->type, rqst->buffer, _qd_con_request_free_cb, rqst);
    ecore_con_url_free(url_complete->url_con);

    return EINA_TRUE;
}
