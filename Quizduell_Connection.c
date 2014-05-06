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

static const char URL_REQUEST_CONTENT_TYPE[] = "application/x-www-form-urlencoded";
static Ecore_Con_Url *url_con = NULL;

Eina_Bool qd_con_init(void)
{
	ecore_con_url_init();
    if (!(url_con = ecore_con_url_new(qd_config.decrypted_base_url)))
        return EINA_FALSE;

    //FIXME: Import QD's actual certificate
    ecore_con_url_ssl_verify_peer_set(url_con, EINA_FALSE);

    return EINA_TRUE;
}

void qd_con_shutdown(void)
{
    ecore_con_url_free(url_con);
    url_con = NULL;
	ecore_con_url_shutdown();
}

static Eina_List *__get_sorted_list_for_params(const Eina_Hash *params_hashlist)
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

Eina_Bool qd_con_request_with_params(const char *api_url, Eina_Hash *params_hashlist, Qd_Con_Request_Type type, Eina_Bool use_post)
{
    Eina_List *sorted_list = NULL;
    Eina_Strbuf *hmac_input_buf = eina_strbuf_new();
    Eina_Strbuf *url_buf = eina_strbuf_new();
    Eina_Strbuf *request_payload = eina_strbuf_new();
    char *hmac = NULL;
    void *hash_element = NULL;
    char time_str[64], *val;
    time_t t;
    struct tm *tmp;
    Eina_Iterator *it = NULL;

    ecore_con_url_additional_headers_clear(url_con);

    eina_strbuf_append(hmac_input_buf, qd_config.decrypted_base_url);
	eina_strbuf_append(hmac_input_buf, api_url);

    // append time string
    t = time(NULL);
    tmp = localtime(&t);
    strftime(time_str, sizeof(time_str), "%F %T", tmp);
    eina_strbuf_append(hmac_input_buf, time_str);

    // append all parameters, sorted
    sorted_list = __get_sorted_list_for_params(params_hashlist);
    EINA_LIST_FREE(sorted_list, val)
    {
        eina_strbuf_append(hmac_input_buf, val);
    }
    sorted_list = NULL;

    hmac = qd_crypto_create_hmac(eina_strbuf_string_get(hmac_input_buf));
    printf("Input for HMAC: \"%s\"\n", eina_strbuf_string_get(hmac_input_buf));
    printf("\tHMAC: \"%s\"\n", hmac);
    ecore_con_url_additional_header_add(url_con, "Authorization", hmac);
    eina_strbuf_free(hmac_input_buf);
    hmac_input_buf = NULL;

    it = eina_hash_iterator_tuple_new(params_hashlist);
    while (eina_iterator_next(it, &hash_element))
    {
        Eina_Hash_Tuple *t = hash_element;
        const char *key = t->key;
        const char *value = t->data;
        eina_strbuf_append_printf(request_payload, "&%s=%s", key, value);
    }
    eina_iterator_free(it); // Always free the iterator after its use
    it = NULL;

    // set device type
    ecore_con_url_additional_header_add(url_con, "dt", "a");

    // set clientdate for the server to validate hash
    ecore_con_url_additional_header_add(url_con, "clientdate", time_str);

    if (use_post)
    {
        ecore_con_url_url_set(url_con, eina_strbuf_string_get(url_buf));
        eina_strbuf_replace_first(request_payload, "&", "");
        ecore_con_url_post(url_con, eina_strbuf_string_get(request_payload), -1, URL_REQUEST_CONTENT_TYPE);
    }
    else
    {
        //use GET
        eina_strbuf_append_buffer(url_buf, request_payload);
        ecore_con_url_url_set(url_con, eina_strbuf_string_get(url_buf));
        ecore_con_url_get(url_con);
    }

    eina_strbuf_free(url_buf);
    eina_strbuf_free(hmac_input_buf);
    eina_strbuf_free(request_payload);
    
    return EINA_TRUE;
}