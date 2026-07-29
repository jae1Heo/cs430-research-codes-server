#ifndef __AUTH_H__
#define __AUTH_H__

#include "game.h"

#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// -lssl -lcrypto required
// sudo apt install libssl-dev
// create a Makefile for this
#define TEST_KEY "abcdefghijklmnopqrstuvwxyzzyxwvu"
#define TEST_IV "zyxwvutsabcdefgh"
#define RSA_SIGNED_SIZE 256
#define PACKET_MAX 64

#define SYMKEY_SIZE 32
#define HASH_SIZE_BEFORE_SIGN 32
#define IV_SIZE 16

extern char* PRIVATE_KEY;
extern char* PUBLIC_KEY;

/*
[rsa encrypted aes key][IV][aes encrypted packet][hash of left three signed with rsa]
    32 bytes            16 bytes    dynamic. max padding required, fixed I guess?
*/

#pragma pack(push, 1)
typedef struct {
    unsigned char symkey[RSA_SIGNED_SIZE];
    unsigned char iv[IV_SIZE];
    unsigned char packet[PACKET_MAX];
    uint16_t packet_len;
    unsigned char hash_signed[RSA_SIGNED_SIZE];
}envelope;
#pragma pack(pop)

EVP_PKEY* load_private_key(const char*);
EVP_PKEY* load_public_key(const char*);
int aes_encrypt(unsigned char*, int, unsigned char*, unsigned char*, unsigned char*);
int generate_iv(unsigned char*);
int rsa_encrypt_aes_key(const unsigned char*, const unsigned char*, size_t, unsigned char*, size_t*);
int generate_signed_hash(const unsigned char*, envelope*);
int build_envelope(unsigned char*, int, envelope*);
int resolve_envelope(const unsigned char*, const size_t, unsigned char*);
int verify_sign(const unsigned char*, const envelope*);
int rsa_decrypt_aes_key(const unsigned char*, const unsigned char*, size_t, unsigned char*, size_t*);
int aes_decrypt(unsigned char*, int, unsigned char*, unsigned char*, unsigned char*);


#endif