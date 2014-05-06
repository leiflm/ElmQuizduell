#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>
#include "Quizduell_Config.h"
#include "Quizduell_Connection.h"

// Account dependent
#define USER_AGENT "Quizduell A 1.3.2"

Quizduell_Config qd_config;

// Config for Germany
Quizduell_Country_Specific_Config de_qcsc = {
    .encrypted_hmac_key = "32O6H/XmXyboLYJ3S+bC5hCrQuSldN7fur9MbU5g33A=",
    .encrypted_base_url = "rt6cSQ+b0gQcNs+Ka/XRtR7MMRvUnkQpmkSlYZLe3Bk="
};

// Config for Italy
Quizduell_Country_Specific_Config it_qcsc = {
    .encrypted_hmac_key = "P/bacQtJsv1nKoMPAacI7BCrQuSldN7fur9MbU5g33A=",
    .encrypted_base_url = "9ddNnke+FSDNh/v5AnM0BX4gdk0WOmIFtkwkEjv+NmA="
};

int main(int arc, char *argv[])
{
    eina_init();
    qd_config_init(&de_qcsc);
    qd_con_init();

    Eina_Hash *hash = eina_hash_string_superfast_new((Eina_Free_Cb)free);
    eina_hash_add(hash, "user", strdup(qd_config.user_name));
    eina_hash_add(hash, "pwd", strdup(qd_config.password_hash));
    qd_con_request_with_params("users/login", hash, QD_CON_LOGIN, EINA_TRUE);
    eina_hash_free(hash);

    qd_con_shutdown();
    qd_config_shutdown();
    eina_shutdown();

    return EXIT_SUCCESS;
}