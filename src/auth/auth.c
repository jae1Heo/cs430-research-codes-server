#include "../../includes/auth.h"

#include "../../includes/auth.h"

// 2048-bit RSA Private Key (Keep this secret! Used for signing/decrypting)
char *PRIVATE_KEY = 
"-----BEGIN PRIVATE KEY-----\n"
"MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQC3eRnLF1pYovxp\n"
"+/9PLF+s3jh/0n050u9/YZWUp1T5uF2EEm1ZfV511JoofMl1Qa5AQi5kzGkPzi9T\n"
"BmLqQPoWZELoLGPi/bO4UKERHUmZUwx7KwjD2Ankyz/9Ixi2J1/TPALx8QIVjhmk\n"
"RHlqDDlwAJODrhXE4CFwEDPfyRxxTSI6P0N65qsn4Fz7Uaa7Lpa3Ol8sVC7ZuVjI\n"
"tBQz1yRx/uQAqJUbPnzg2oVC9hKSlzrdd/iRxS83dp8meOMl2+6+ioJiZeHWLZC1\n"
"6jYc5ijTR1ZhLkhhpHYTkZF0UjtrBphtTx+9vBepsRwAlrgBcTTKTdBTjLekvtKu\n"
"N4ZXvdX3AgMBAAECggEAGP1zMHDtWt5UUCFFTKToITUhb/OdH+elvG1vwNAr6Fme\n"
"zzLtVv+XAqijGLmqfQCludXp2Mvo0YDNfFsF1Q9loPAmFtaZAl15AbVlwt2A3VPt\n"
"AsfBSV+fNV0jyZhisMjbKfvmEMs5c/MndfLnsLSajDFf5lMrXuYCV++1CRms+JeE\n"
"WSDPynI1+mGA0kUGvc618KFWrjhznoBiAMba+qjK+zLSbTnNG87TZa5wRcwLckr1\n"
"yTz2fVvcOqeNhr2rGJWOTYjNXDodrVFWs2lJ/CxuF0qgmwfhJvcG8tMTwWVAw3XH\n"
"cvJ38O4zYLxKfWKOkS7K9bMB6GTUdoCWe9gX29D3kQKBgQDmLM7CCfF9O0yiwHPs\n"
"F7vIhj9YRLWAZohE9yIepDhOqSGogaoI4UZx4AHQGhLlIAu4cUZnnic7L2itm6+Z\n"
"AgHe4mPwOPabnsVtGhcpGkNfaapcaf5gnl6K+Ix0y6iPywmGEiHdskPdhiYtkoYT\n"
"rDfYfe8uahqrzMuXPs8YDFWqZwKBgQDMDuXCuIZ3tjBDm3IkCYu8A3z9Xg8T40Dk\n"
"wJxcu2eq7LQs4geOvz2RiN/DkGErktqpCGJ/80b0O1aRKLp3qQCfpvLhdR5J3EtU\n"
"swpCnJ0v1P6ASrbRnOyzmi4tb8dGzI3QwveaenOzjT9T9i/YfhSw190S1kCuGQyZ\n"
"TWg9Fadd8QKBgESzcQ73es2/KlOVj8QTHWhvsW5+K5JkiB3ViaiNqYTI9JPgB8lY\n"
"u/b3PnNHpQYgd38YyEPbL6h5sQvIYTQRJk/JULz9s5WlYRX8aJdLEHrlBgVJWbSq\n"
"8FsYbovZ0bkBiBktRlBuidUP2ktV5ZDvoUeL5xz6bHWEzeuZwqRR4ivXAoGBAKY3\n"
"EFJPSkDi52d6jDvshqTsVnhvkGqLtwVeGz+x61ygGJdiufM9taIBUvuSdIQwx+ne\n"
"CZXdjRNN7lKpUVI9GGPzcm3Rn41cYwQoHphpx+P5ZCEOOuJ9J5WYZjrejP0PjAjb\n"
"dyKe0fYUfNwgcZKwCJyQCOTaHq9xegC5u8neBImRAoGAJ7dAQ+D/D/pU9LQbCcR7\n"
"fK+u3csoK280e7JNQSHlYFqdW9e74oa3WOoF0K0rnlLJVY0LM4es6MnMc8uWNFR1\n"
"iDmQ9qGxcEyPd4Gi9+ekgS9E9XXHtoJCxpxhtWp19Hjc6SsceKQ4aRpx3gO4+slZ\n"
"oEIPo7CMm/V+FXzV97UsQc0=\n"
"-----END PRIVATE KEY-----\n";

// 2048-bit RSA Public Key (Share this! Used for verifying/encrypting)
char *PUBLIC_KEY = 
"-----BEGIN PUBLIC KEY-----\n"
"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAt3kZyxdaWKL8afv/Tyxf\n"
"rN44f9J9OdLvf2GVlKdU+bhdhBJtWX1eddSaKHzJdUGuQEIuZMxpD84vUwZi6kD6\n"
"FmRC6Cxj4v2zuFChER1JmVMMeysIw9gJ5Ms//SMYtidf0zwC8fECFY4ZpER5agw5\n"
"cACTg64VxOAhcBAz38kccU0iOj9DeuarJ+Bc+1Gmuy6WtzpfLFQu2blYyLQUM9ck\n"
"cf7kAKiVGz584NqFQvYSkpc63Xf4kcUvN3afJnjjJdvuvoqCYmXh1i2Qteo2HOYo\n"
"00dWYS5IYaR2E5GRdFI7awaYbU8fvbwXqbEcAJa4AXE0yk3QU4y3pL7SrjeGV73V\n"
"9wIDAQAB\n"
"-----END PUBLIC KEY-----\n";


int generate_iv(unsigned char* iv_buffer) {
    memset(iv_buffer, 0, IV_SIZE);

    // generate random 16 bytes for IV
    if(RAND_bytes(iv_buffer, IV_SIZE) != 1) {
        return 0;
    }
    return 1;
}

int aes_encrypt(unsigned char* plaintext, int plaintext_len, unsigned char* iv, unsigned char* key, unsigned char* ciphertext) {
    // allocate new cipher context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

    int len;
    int ciphertext_len;

    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    // encrypt blocks
    EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len);
    ciphertext_len = len;

    // add PKCS#7 padding
    EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

EVP_PKEY* load_private_key(const char* key_str) {
    BIO *bio = BIO_new_mem_buf(key_str, -1);
    if (!bio) return NULL;
    
    // Reads the PEM string and constructs the key object
    EVP_PKEY *pkey = PEM_read_bio_PrivateKey(bio, NULL, NULL, NULL);
    BIO_free(bio);
    return pkey;
}

EVP_PKEY* load_public_key(const char* key_str) {
    BIO *bio = BIO_new_mem_buf(key_str, -1);
    if (!bio) return NULL;
    
    // Reads the PEM string and constructs the key object
    EVP_PKEY *pkey = PEM_read_bio_PUBKEY(bio, NULL, NULL, NULL);
    BIO_free(bio);
    return pkey;
}

int rsa_encrypt_aes_key(const unsigned char* public_key, const unsigned char* symKey, size_t symKey_len, unsigned char* enc_sym, size_t* key_len) {
    
    EVP_PKEY* pubKey = load_public_key(public_key);
    if(!pubKey) {
        return 0;
    }

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pubKey, NULL);
    if(!ctx) {
        EVP_PKEY_free(pubKey);
        return 0;
    }

    if(EVP_PKEY_encrypt_init(ctx) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(pubKey);
        return 0;
    }

    if(EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(pubKey);
        return 0;
    }

    // to calculate size of encrypted sym key.
    if(EVP_PKEY_encrypt(ctx, NULL, key_len, symKey, symKey_len) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(pubKey);
        return 0;
    }

    // to encrypt actual symmetric key
    if(EVP_PKEY_encrypt(ctx, enc_sym, key_len, symKey, symKey_len) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(pubKey);
        return 0;
    }

    EVP_PKEY_free(pubKey);
    EVP_PKEY_CTX_free(ctx);
    return 1;

}

int generate_signed_hash(const unsigned char* private_key, envelope* env) {
    if(env->packet_len <= 0) {
        return 0;
    }
    
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    EVP_PKEY* priv_key = load_private_key(private_key);
    
    if(!priv_key) {
        EVP_MD_CTX_free(mdctx);
        return 0;
    }

    size_t total_data_to_digest = RSA_SIGNED_SIZE + IV_SIZE + env->packet_len;

    unsigned char* data_buffer = (unsigned char*)malloc(total_data_to_digest);
    memcpy(data_buffer, env->symkey, RSA_SIGNED_SIZE);
    memcpy(data_buffer + RSA_SIGNED_SIZE, env->iv, IV_SIZE);
    memcpy(data_buffer + RSA_SIGNED_SIZE + IV_SIZE, env->packet, env->packet_len);

    if(EVP_DigestSignInit(mdctx, NULL, EVP_sha256(), NULL, priv_key) <= 0) {
        EVP_MD_CTX_free(mdctx);
        EVP_PKEY_free(priv_key);
        return 0;
    }


    if(EVP_DigestSignUpdate(mdctx, data_buffer, total_data_to_digest) <= 0){
        EVP_MD_CTX_free(mdctx);
        EVP_PKEY_free(priv_key);
        return 0;
    }

    size_t sign_len = RSA_SIGNED_SIZE;
    if(EVP_DigestSignFinal(mdctx, env->hash_signed, &sign_len) <= 0) {
        EVP_MD_CTX_free(mdctx);
        EVP_PKEY_free(priv_key);
        return 0;
    }
    
    EVP_MD_CTX_free(mdctx);
    EVP_PKEY_free(priv_key);
    free(data_buffer);
    return 1;
}

int build_envelope(unsigned char* plaintext, int plaintext_len, envelope* packet_buffer) {
    // generate IV first
    unsigned char dynamic_iv[IV_SIZE];
    if(!generate_iv(dynamic_iv)) {
        return 0;
    }
    memcpy(packet_buffer->iv, dynamic_iv, IV_SIZE);

    unsigned char symkey_buffer[SYMKEY_SIZE];
    memset(symkey_buffer, 0, SYMKEY_SIZE);
    memcpy(symkey_buffer, TEST_KEY, SYMKEY_SIZE);

    // first generate aes cipher of plaintext data with symmkey 
    int ciphertext_len = aes_encrypt(plaintext, plaintext_len, dynamic_iv, symkey_buffer, packet_buffer->packet);
    // save the size of the packet len to buffer
    packet_buffer->packet_len = ciphertext_len;
    size_t keySize = 0;

    
    if(!rsa_encrypt_aes_key(PUBLIC_KEY, symkey_buffer, 32, packet_buffer->symkey, &keySize)) {
        return 0;
    }

    if(!generate_signed_hash(PRIVATE_KEY, packet_buffer)) {
        return 0;
    }

    return 1;
}

// have to pass sender's public key
int verify_sign(const unsigned char* public_key, const envelope* env) {
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if(!mdctx) {
        return 0;
    }

    EVP_PKEY* pkey = load_public_key(public_key);
    if(!pkey) {
        EVP_MD_CTX_free(mdctx);
        return 0;
    }

    // construct the payload
    size_t total_data_to_digest = RSA_SIGNED_SIZE + IV_SIZE + env->packet_len;
    unsigned char* data_buffer = (unsigned char*)malloc(total_data_to_digest);
    if(data_buffer == NULL) {
        EVP_MD_CTX_free(mdctx);
        EVP_PKEY_free(pkey);
        return 0;
    }

    memcpy(data_buffer, env->symkey, RSA_SIGNED_SIZE);
    memcpy(data_buffer + RSA_SIGNED_SIZE, env->iv, IV_SIZE);
    memcpy(data_buffer + RSA_SIGNED_SIZE + IV_SIZE, env->packet, env->packet_len);

    // initialize the verifier with sha256, and sender's public key
    if(EVP_DigestVerifyInit(mdctx, NULL, EVP_sha256(), NULL, pkey) <= 0) {
        EVP_MD_CTX_free(mdctx);
        EVP_PKEY_free(pkey);
        free(data_buffer);
        return 0;
    }

    // feed the reconstructed payload into the context
    if(EVP_DigestVerifyUpdate(mdctx, data_buffer, total_data_to_digest) <= 0) {
        EVP_MD_CTX_free(mdctx);
        EVP_PKEY_free(pkey);
        free(data_buffer);
        return 0;
    }

    // perform final verification. return 1 if success, 0 if fail
    int auth_state = EVP_DigestVerifyFinal(mdctx, env->hash_signed, RSA_SIGNED_SIZE);

    EVP_MD_CTX_free(mdctx);
    EVP_PKEY_free(pkey);
    free(data_buffer);

    if(auth_state) {
        return 1;
    }
    else {
        return 0;
    }
}

int rsa_decrypt_aes_key(const unsigned char* private_key, const unsigned char* enc_sym, size_t encsym_size, unsigned char* sym_key, size_t* symkey_size) {
    EVP_PKEY* priv_key = load_private_key(private_key);
    
    if(!priv_key) {
        return 0;
    }

    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(priv_key, NULL);
    if(!ctx) {
        EVP_PKEY_free(priv_key);
        return 0;
    }

    if(EVP_PKEY_decrypt_init(ctx) <= 0) {
        EVP_PKEY_free(priv_key);
        EVP_PKEY_CTX_free(ctx);
        return 0;
    }

    if(EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0) {
        EVP_PKEY_free(priv_key);
        EVP_PKEY_CTX_free(ctx);
        return 0;
    }

    if(EVP_PKEY_decrypt(ctx, NULL, symkey_size, enc_sym, encsym_size) <= 0) {
        EVP_PKEY_free(priv_key);
        EVP_PKEY_CTX_free(ctx);
        return 0;
    }

    if(EVP_PKEY_decrypt(ctx, sym_key, symkey_size, enc_sym, encsym_size) <= 0) {
        EVP_PKEY_free(priv_key);
        EVP_PKEY_CTX_free(ctx);
        return 0;
    }

    EVP_PKEY_free(priv_key);
    EVP_PKEY_CTX_free(ctx);

    return 1;
}

int aes_decrypt(unsigned char* ciphertext, int ciphertext_len, unsigned char* iv, unsigned char* key, unsigned char* plaintext) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    int len;
    int plaintext_len;

    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    // decrypt blocks
    EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len);
    plaintext_len = len;

    // remove PKCS#7 padding
    // will fail if key/iv is different
    if(EVP_DecryptFinal_ex(ctx, plaintext + len, &len) <= 0) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return plaintext_len;
}

// passes empty envelope variable and game_data variable to copy
int resolve_envelope(const unsigned char* encrypted_envelope, const size_t encrypted_envelope_size, unsigned char* dec_data) {
    envelope env;
    memset(&env, 0, sizeof(envelope));
    if(encrypted_envelope_size == sizeof(envelope)) {
        memcpy(&env, encrypted_envelope, sizeof(envelope));
    }
    else {
        return 0;
    }

    if(!verify_sign(PUBLIC_KEY, &env)) {
        return 0;
    }

    unsigned char* decrypted_rsa_key = (unsigned char*)malloc(SYMKEY_SIZE);
    memset(decrypted_rsa_key, 0, SYMKEY_SIZE);

    size_t keysize = 0;
    if(!rsa_decrypt_aes_key(PRIVATE_KEY, env.symkey, RSA_SIGNED_SIZE, decrypted_rsa_key, &keysize)) {
        free(decrypted_rsa_key);
        return 0;
    }

    unsigned char* plaintext = (unsigned char*)malloc(env.packet_len);
    memset(plaintext, 0, sizeof(struct game_data));

    size_t plaintext_len = aes_decrypt(env.packet, env.packet_len, env.iv, decrypted_rsa_key, plaintext);
    if(plaintext_len < 0) {
        free(decrypted_rsa_key);
        free(plaintext);
        return 0;
    }

    if(plaintext_len != sizeof(struct game_data)) {
        free(decrypted_rsa_key);
        free(plaintext);
        return 0;
    }
    else {
        memcpy(dec_data, plaintext, sizeof(struct game_data));
    }

    free(decrypted_rsa_key);
    free(plaintext);
    return 1;
}


/*
int aes_encrypt(unsigned char* plaintext, int plaintext_len, unsigned char* ciphertext) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    int len;
    int ciphertext_len;

    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, TEST_KEY, TEST_IV);

    // encrypt blocks
    EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len);
    ciphertext_len = len;

    // add PKCS#7 padding
    EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int aes_decrypt(unsigned char* ciphertext, int ciphertext_len, unsigned char* plaintext) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    int len;
    int plaintext_len;

    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, TEST_KEY, TEST_IV);

    // decrypt blocks
    EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len);
    plaintext_len = len;

    // remove PKCS#7 padding
    // will fail if key/iv is different
    if(EVP_DecryptFinal_ex(ctx, plaintext + len, &len) <= 0) {
        return -1;
    }
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return plaintext_len;

}
*/