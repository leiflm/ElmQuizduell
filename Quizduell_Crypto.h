#ifndef __QUIZDUELL_CRYPTO_H__
#define __QUIZDUELL_CRYPTO_H__

void aes_decrypt(char **output_data, const char *base64_encoded_input_data);
void create_hmac(char **hmac, const char *input_data);
void create_password_hash(char **hash, const char *password);

#endif