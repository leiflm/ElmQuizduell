#ifndef __QUIZDUELL_CONFIG_H__
#define __QUIZDUELL_CONFIG_H__

typedef struct _Quizduell_Country_Specific_Config {
    const char *encrypted_hmac_key; // apk_decompilation/res/values/strings.xml:android_handy_id
    const char *encrypted_base_url; // apk_decompilation/res/values/strings.xml:android_twiggly_id
} Quizduell_Country_Specific_Config;

typedef struct _Quizduell_Config {
    char *decrypted_hmac_key;
    char *decrypted_base_url;
    char user_name[128];
    char password[128];
    char *password_hash;
} Quizduell_Config;

extern Quizduell_Config qd_config;

void quizduell_config_init(const Quizduell_Country_Specific_Config *qcsc);
void quizduell_config_free(void);

#endif