#include "../../includes/auth.h"

int generate_symmetric_key(unsigned char *key, size_t key_len) {
    if(!key || key_len != AES_KEY_SIZE) {
        return 0;
    }

    if(!RAND_bytes(key, (int)key_len)) {
        return 0;
    }

    return 1;
}

int generate_iv(unsigned char *iv, size_t iv_len) {
    if(!iv || iv_len != AES_GCM_IV_SIZE) {
        return 0;
    }

    if(!RAND_bytes(iv, (int)iv_len)) {
        return 0;
    }

    return 1;
}

int encrypt_packet(const packet_data plaintext[PACKET_SIZE],
                   unsigned char *key,
                   unsigned char *iv,
                   unsigned char *ciphertext,
                   unsigned char *tag) {
    if(!plaintext || !key || !iv || !ciphertext || !tag) {
         return 0;
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if(!ctx) {
        return 0;
    }

    int len, ciphertext_len = 0;

    if(!EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL)) {return 0;}
    if(!EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv)) {return 0;}

    if(!EVP_EncryptUpdate(ctx, ciphertext, &len, (unsigned char*)plaintext, sizeof(packet_data)*PACKET_SIZE)) {return 0;}
    ciphertext_len = len;

    if(!EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) {return 0;}
    ciphertext_len += len;

    if(!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, AES_GCM_TAG_SIZE, tag)) {return 0;}

    EVP_CIPHER_CTX_free(ctx);
    return ciphertext_len;

}

int decrypt_packet(const unsigned char *ciphertext,
                   int ciphertext_len,
                   unsigned char *key,
                   unsigned char *iv,
                   unsigned char *tag,
                   packet_data plaintext[PACKET_SIZE]) {


}

