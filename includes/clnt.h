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

#define MAX_CLIENTS 2 // number of max clients 


typedef struct one_way_channel { // struct for one way mutex channel
    unsigned char* data;
    unsigned int has_data;

    pthread_mutex_t mutex;
    pthread_cond_t cond;
} one_way;

typedef struct two_way_channel { // struct for bi-directional mutex channel
    struct one_way_channel command;
    struct one_way_channel response;
} two_way;

typedef struct client_info{ // struct for holding client information
    struct sock clnt_sock;
    struct two_way_channel* channel;
    int clinet_id;
    unsigned char key[AES_KEY_SIZE];
    unsigned char iv[AES_GCM_IV_SIZE];
} client_info_t;

int init_channel(struct one_way_channel*); // initialize one way mutex
int init_twoway(struct two_way_channel*); // initialize bi-directional mutex
void* client_main(void*); // function that that client thread will run
void channel_send(struct one_way_channel*, const unsigned char[PACKET_MAX]); // send data to the parent process
void channel_receive(struct one_way_channel*, unsigned char[PACKET_MAX]); // receive data from the parent process
void close_twoway(struct two_way_channel*); // clean the one way mutex
void close_channel(struct one_way_channel*); // clean the bi-directional mutex

#endif