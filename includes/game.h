#ifndef __GAME_H__
#define __GAME_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>


#define PACKET_INVALID 0xFFFF

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


#define BALL_SPEED 150.0f
#define PADDLE_SPEED 300.0f
#define PADDLE_WIDTH 15.0f
#define PADDLE_HEIGHT 90.0f
#define BALL_SIZE 15.0f

#define TICK_RATE 60.0f
#define TICK_DT (1.0f / TICK_RATE)

#define RAW_PACKET_SIZE 9
#define PACKET_MAX 256

/*
game data will be packed as packet_data variable and sent to the clients
*/


typedef struct game_data{
    uint16_t left_score; // 16 bit -> 2 byte
    uint16_t right_score; // 16 bit -> 2 byte

    float ball_pos_x; // 32 bit -> 4 byte
    float ball_pos_y; // 32 bit -> 4 byte
    float ball_vel_x; // 32 bit -> 4 byte
    float ball_vel_y; // 32 bit -> 4 byte

    float left_paddle_y; // 32 bit -> 4 byte
    float left_paddle_x; // 32 bit -> 4 byte
    float right_paddle_y; // 32 bit -> 4 byte
    float right_paddle_x; // 32 bit -> 4 byte

    double server_time;

    // 16 bit -> 2 byte
    uint16_t game_status; //1 = running, else = not

}data;

typedef struct player_mv{
    uint8_t player_w;
    uint8_t player_s;
}mv;


void reset(struct game_data*);
double time_now_sec();
void run(struct game_data*);
void update(struct game_data*, const unsigned int, const unsigned int, const unsigned int, const unsigned int, double);
int pack_data(struct game_data*, char*, size_t);
void unpack_data(struct player_mv*, const unsigned char*);

#endif 