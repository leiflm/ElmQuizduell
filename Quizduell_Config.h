#ifndef __QUIZDUELL_CONFIG_H__
#define __QUIZDUELL_CONFIG_H__

#include <Eina.h>

typedef struct _Quizduell_Country_Specific_Config {
    const char *encrypted_hmac_key; // apk_decompilation/res/values/strings.xml:android_handy_id
    const char *encrypted_base_url; // apk_decompilation/res/values/strings.xml:android_twiggly_id
} Quizduell_Country_Specific_Config;

typedef struct _Quizduell_Config {
    char *decrypted_hmac_key;
    char *decrypted_base_url;
    Eina_Stringshare *username;
    Eina_Stringshare *password;
} Quizduell_Config;

extern Quizduell_Config qd_config;

void qd_config_init(const Quizduell_Country_Specific_Config *qcsc);
void qd_config_shutdown(void);

#endif