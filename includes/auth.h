#ifndef __AUTH_H__
#define __AUTH_H__

#include "game.h"

#include <openssl/evp.h>
#include <stdio.h>
#include <string.h>
// -lssl -lcrypto required
// sudo apt install libssl-dev
// create a Makefile for this
#define TEST_KEY "abcdefghijklmnopqrstuvwxyzzyxwvu"
#define TEST_IV "zyxwvutsabcdefgh"

int aes_encrypt(unsigned char*, int, unsigned char*);
int aes_decrypt(unsigned char*, int, unsigned char*);

#endif