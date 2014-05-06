#include <stdlib.h>
#include <stdio.h>
#include "Quizduell_Config.h"
#include "Quizduell_Crypto.h"

void qd_config_init(const Quizduell_Country_Specific_Config *qcsc)
{
	// set some initial values
    snprintf(qd_config.user_name, sizeof(qd_config.user_name), "reverseengineer");
    snprintf(qd_config.password, sizeof(qd_config.password), "reverseengineer");

    // create hash/decrypt other values
    qd_config.password_hash = qd_crypto_create_password_hash(qd_config.password);
    qd_config.decrypted_base_url = qd_crypto_aes_decrypt(qcsc->encrypted_base_url);
    qd_config.decrypted_hmac_key = qd_crypto_aes_decrypt(qcsc->encrypted_hmac_key);
}

void qd_config_shutdown(void)
{
    free(qd_config.decrypted_base_url);
    qd_config.decrypted_base_url = NULL;
    free(qd_config.decrypted_hmac_key);
    qd_config.decrypted_hmac_key = NULL;
    free(qd_config.password_hash);
    qd_config.password_hash = NULL;
}