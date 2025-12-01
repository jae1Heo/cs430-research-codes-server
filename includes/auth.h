#ifndef __AUTH_H__
#define __AUTH_H__

#include "game.h"

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <string.h>
// -lssl -lcrypto required
// sudo apt install libssl-dev
// create a Makefile for this

#define AES_KEY_SIZE 32 // 256-bit
#define AES_GCM_IV_SIZE 12 // 96-bit IV recommanded gcm size
#define AES_GCM_TAG_SIZE 16 // 128-bit gcm tag size
#define SHA256_HASH_SIZE 32 // SHA256 hash size

// Key and IV generation
int generate_symmetric_key(unsigned char *key, size_t key_len);
int generate_iv(unsigned char *iv, size_t iv_len);

// Packet encryption/decryption
int encrypt_packet(const packet_data plaintext[PACKET_SIZE],
                   unsigned char *key,
                   unsigned char *iv,
                   unsigned char *ciphertext,
                   unsigned char *tag);

int decrypt_packet(const unsigned char *ciphertext,
                   int ciphertext_len,
                   unsigned char *key,
                   unsigned char *iv,
                   unsigned char *tag,
                   packet_data plaintext[PACKET_SIZE]);

// Packet hashing
int hash_packet(const packet_data packet[PACKET_SIZE], unsigned char output[SHA256_HASH_SIZE]);
int verify_hash(const unsigned char hash1[SHA256_HASH_SIZE], const unsigned char hash2[SHA256_HASH_SIZE]);

// verify attestation
int verify_key_attestation(const unsigned char *attestation, size_t att_size, EVP_PKEY **client_pubkey);

#endif