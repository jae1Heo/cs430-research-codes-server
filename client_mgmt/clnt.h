#ifndef __CLNT_H__
#define __CLNT_H__

#include "../server/server.h"
#include "../logic/game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define PACKET_SIZE 8
typedef uint16_t data_size;


/*

player 1 x, y uint16_t
player 2 x, y uint16_t
player 1 score uint16_t
player 2 score uint16_t
ball x uint16_t
ball y uint16_t
stop game uint16_t
fin zeros uint16_t*/

enum {player1_x = 0, player1_y, player2_x, player2_y, player1_score, player2_score, ball_x, ball_y, stop_game, reserved};

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

int init_channel(struct one_way_channel*); // initialize one way mutex
int init_twoway(struct two_way_channel*); // initialize two way mutex
void* client_main(void*); // client work
void channel_send(struct one_way_channel*, const data_size[PACKET_SIZE]); // send data to the parent process
void channel_receive(struct one_way_channel*, data_size[PACKET_SIZE]); // receive data from the parent process
void close_twoway(struct two_way_channel* channel);
void close_channel(struct one_way_channel* channel);

#endif