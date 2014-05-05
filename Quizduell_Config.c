#include <stdlib.h>
#include <stdio.h>
#include "Quizduell_Config.h"
#include "Quizduell_Crypto.h"

void quizduell_config_init(const Quizduell_Country_Specific_Config *qcsc)
{
	// set some initial values
    snprintf(qd_config.user_name, sizeof(qd_config.user_name), "reverseengineer");
    snprintf(qd_config.password, sizeof(qd_config.password), "reverseengineer");

    // create hash/decrypt other values
    create_password_hash(&qd_config.password_hash, qd_config.password);
    aes_decrypt(&qd_config.decrypted_base_url, qcsc->encrypted_base_url);
    aes_decrypt(&qd_config.decrypted_hmac_key, qcsc->encrypted_hmac_key);
}

void quizduell_config_free(void)
{
    free(qd_config.decrypted_base_url);
    free(qd_config.decrypted_hmac_key);
    free(qd_config.password_hash);
}