#define _GNU_SOURCE
#include <stdio.h>
#include <math.h> // for abs(x) prepare_key
#include <gcrypt.h>
#include <Eina.h>
#include "base64.h"
#include "Quizduell_Config.h"
#include "Quizduell_Crypto.h"

#define RAW_AES_KEY "se.feomedia926155691629764696se.feomedia"

// 16 bytes will be written into ouput_key
static void _prepare_key(const char *input_key, char *output_key)
{
  int i = 0;
    size_t input_key_length = strlen(input_key);
    for (i = 0; i < 16; i++) {
        output_key[i] = ((i + input_key[abs((input_key_length - i) % input_key_length)]) % 256);
    }
}

char *qd_crypto_aes_decrypt(const char *base64_encoded_input_data)
{
    #define GCRY_CIPHER_MODE GCRY_CIPHER_MODE_ECB
    #define GCRY_CIPHER GCRY_CIPHER_AES128   // Pick the cipher here
 
    char *output_data = NULL;
    char aesSymKey[16];
    gcry_error_t     gcryError;
    gcry_cipher_hd_t gcryCipherHd;
    size_t           key_length = gcry_cipher_get_algo_keylen(GCRY_CIPHER);
    size_t           output_data_length;
    
    base64_decode_alloc(base64_encoded_input_data, strlen(base64_encoded_input_data), &output_data, &output_data_length);
    _prepare_key(RAW_AES_KEY, aesSymKey);
 
    gcryError = gcry_cipher_open(&gcryCipherHd, GCRY_CIPHER, GCRY_CIPHER_MODE, 0);
    if (gcryError)
    {
        printf("gcry_cipher_open failed:  %s/%s\n",
               gcry_strsource(gcryError),
               gcry_strerror(gcryError));
        goto _decrypt_failed;
    }
 
    gcryError = gcry_cipher_setkey(gcryCipherHd, aesSymKey, key_length);
    if (gcryError)
    {
        printf("gcry_cipher_setkey failed:  %s/%s\n",
               gcry_strsource(gcryError),
               gcry_strerror(gcryError));
        goto _decrypt_failed;
    }
  

    gcryError = gcry_cipher_decrypt(gcryCipherHd, output_data, output_data_length, NULL, 0);
    if (gcryError)
    {
        printf("gcry_cipher_decrypt failed:  %s/%s\n",
               gcry_strsource(gcryError),
               gcry_strerror(gcryError));
        goto _decrypt_failed;
    }

    // clean up after ourselves
    gcry_cipher_close(gcryCipherHd);

    return output_data;

    _decrypt_failed:
      return NULL;
}

char *qd_crypto_create_hmac(const char *input_data)
{
  #define MESSAGE_DIGEST_ALGO GCRY_MD_SHA256
  const size_t HMAC_KEY_LEN = 16;
  char *hmac = NULL;
  size_t hmac_len = gcry_md_get_algo_dlen(MESSAGE_DIGEST_ALGO);

  gcry_md_hd_t hd;
  gcry_error_t err;
  unsigned char *hmac_ptr;

  /* Create a hash device and generate the HMAC */
  hd = NULL;
  err = gcry_md_open(&hd, MESSAGE_DIGEST_ALGO, GCRY_MD_FLAG_HMAC);
  if (err)
  {
    printf("Creating HMAC-SHA-256 object failed: %s",
        gcry_strerror (err));
    goto __hashing_failed;
  }

  err = gcry_md_setkey(hd, qd_config.decrypted_hmac_key, HMAC_KEY_LEN);
  if (err != 0)
  {
    printf("gcry_md_setkey failed: %s", gcry_strerror (err));
    gcry_md_close (hd);
    goto __hashing_failed;
  }

  gcry_md_write(hd, input_data, strlen(input_data));

  hmac_ptr = gcry_md_read(hd, MESSAGE_DIGEST_ALGO);
  if (hmac_ptr == NULL)
  {
    printf("gcry_md_read failed.");
    gcry_md_close (hd);
    goto __hashing_failed;
  }

  base64_encode_alloc((const char*)hmac_ptr, hmac_len, &hmac);

  /* Clean up */
  gcry_md_close(hd);
  hd = NULL;

  return hmac;

  __hashing_failed:
    return NULL;
  #undef MESSAGE_DIGEST_ALGO
}

char *qd_crypto_create_password_hash(const char *password)
{
  #define MESSAGE_DIGEST_ALGO GCRY_MD_MD5
  const char PWD_HASH_PADDING[] = "SQ2zgOTmQc8KXmBP";
  char *hash = NULL;
  size_t hash_len = gcry_md_get_algo_dlen(MESSAGE_DIGEST_ALGO);
  size_t expanded_hash_len = (hash_len * 2) + 1;
  char *hash_buffer = malloc(expanded_hash_len);
  hash = malloc(expanded_hash_len);
  char *buffer = NULL;
  size_t input_len = 0;
  Eina_Strbuf *expanded_hash = eina_strbuf_new();
  int i = 0;
  
  input_len = asprintf(&buffer, "%s%s", PWD_HASH_PADDING, password);

  /* Create a hash device and check the HASH */
  gcry_md_hash_buffer(MESSAGE_DIGEST_ALGO, (void *)(hash_buffer), (const void*)buffer, input_len);
  for (i = 0; i < hash_len; i++) {
    eina_strbuf_append_printf(expanded_hash, "%02x", (int)(0xff & hash_buffer[i]));
  }

  hash = eina_strbuf_string_steal(expanded_hash);
  eina_strbuf_free(expanded_hash);

  return hash;

  #undef MESSAGE_DIGEST_ALGO
}