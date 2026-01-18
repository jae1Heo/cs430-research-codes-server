#ifndef __CLNT_H__
#define __CLNT_H__

#include "server.h"
#include "auth.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

typedef uint16_t data_size;

#define MAX_CLIENTS 2

/*

player 1 x, y uint16_t
player 2 x, y uint16_t
player 1 score uint16_t
player 2 score uint16_t
ball x uint16_t
ball y uint16_t
stop game uint16_t
fin zeros uint16_t*/

typedef struct one_way_channel {
    data_size* data;
    unsigned int has_data;

    pthread_mutex_t mutex;
    pthread_cond_t cond;
} one_way;

typedef struct two_way_channel {
    struct one_way_channel command;
    struct one_way_channel response;
} two_way;

typedef struct client_info{
    struct sock clnt_sock;
    struct two_way_channel* channel;
    int clinet_id;
    unsigned char key[AES_KEY_SIZE];
    unsigned char iv[AES_GCM_IV_SIZE];
} client_info_t;

int init_channel(struct one_way_channel*); // initialize one way mutex
int init_twoway(struct two_way_channel*); // initialize two way mutex
void* client_main(void*); // client work
void channel_send(struct one_way_channel*, const data_size[PACKET_SIZE]); // send data to the parent process
void channel_receive(struct one_way_channel*, data_size[PACKET_SIZE]); // receive data from the parent process
void close_twoway(struct two_way_channel*);
void close_channel(struct one_way_channel*);

#endif