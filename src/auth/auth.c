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
    if(!ciphertext || !key || !iv || !tag || !plaintext) 
    {
        return 0;
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if(!ctx) return 0;

    int len, plaintext_len = 0;

    if(!EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL)) {return 0;}
    if(!EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv)) {return 0;}

    if(!EVP_DecryptUpdate(ctx, (unsigned char*)plaintext, &len, ciphertext, ciphertext_len)) {return 0;}
    plaintext_len = len;

    if(!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, AES_GCM_TAG_SIZE, tag)) {return 0;}

    if(EVP_DecryptFinal_ex(ctx, (unsigned char*)plaintext + len, &len) <= 0) {return 0;}
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return plaintext_len;

}

int hash_packet(const packet_data packet[PACKET_SIZE], unsigned char output[SHA256_HASH_SIZE]) {
    if(!packet || !output) 
    {
        return 0;
    }

    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, packet, sizeof(packet_data)*PACKET_SIZE);
    SHA256_Final(output, &sha256);

    return 1;
}

int verify_hash(const unsigned char hash1[SHA256_HASH_SIZE], const unsigned char hash2[SHA256_HASH_SIZE]) {
    if(!hash1 || !hash2) {
        return 0;
    }

    return (memcmp(hash1, hash2, SHA256_HASH_SIZE) == 0);
}

int verify_key_attestation(const unsigned char *attestation, size_t att_size, EVP_PKEY **client_pubkey) {
    // finish after client enclave is ready
    return 1;
}
