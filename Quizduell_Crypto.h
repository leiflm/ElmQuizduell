#ifndef __QUIZDUELL_CRYPTO_H__
#define __QUIZDUELL_CRYPTO_H__

char *qd_crypto_aes_decrypt(const char *base64_encoded_input_data);
char *qd_crypto_create_hmac(const char *input_data);
char *qd_crypto_create_password_hash(const char *password);

#endif