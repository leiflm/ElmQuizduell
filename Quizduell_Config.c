#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Quizduell_Config.h"
#include "Quizduell_Crypto.h"

void qd_config_init(const Quizduell_Country_Specific_Config *qcsc)
{
    char *last_slash = NULL;

    qd_config.decrypted_base_url = qd_crypto_aes_decrypt(qcsc->encrypted_base_url);
    if ((last_slash = strrchr(qd_config.decrypted_base_url, '/')))
    {
        last_slash[1] = '\0'; // NULL-terminate after last slash
    }
    qd_config.decrypted_hmac_key = qd_crypto_aes_decrypt(qcsc->encrypted_hmac_key);
    qd_config.user_agent = eina_stringshare_add(qcsc->user_agent);
    qd_config.pem_file = eina_stringshare_add(qcsc->pem_file);
}

void qd_config_shutdown(void)
{
    free(qd_config.decrypted_base_url);
    qd_config.decrypted_base_url = NULL;
    free(qd_config.decrypted_hmac_key);
    qd_config.decrypted_hmac_key = NULL;

    eina_stringshare_del(qd_config.username), qd_config.username = NULL;
    eina_stringshare_del(qd_config.password), qd_config.password = NULL;
    eina_stringshare_del(qd_config.user_agent), qd_config.user_agent = NULL;
    eina_stringshare_del(qd_config.pem_file), qd_config.pem_file = NULL;
}