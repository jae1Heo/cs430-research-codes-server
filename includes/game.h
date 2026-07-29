#ifndef __GAME_H__
#define __GAME_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <string.h>


#define PACKET_INVALID 0xFFFF

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


#define BALL_SPEED 150.0f
#define PADDLE_SPEED 300.0f
#define PADDLE_WIDTH 15.0f
#define PADDLE_HEIGHT 90.0f
#define PADDLE_WIDTH_MARGIN 20.0f
#define BALL_SIZE 15.0f

#define TICK_RATE 60.0f
#define TICK_DT (1.0f / TICK_RATE)

#define PACKET_MAX 64

/*
game data will be packed as packet_data variable and sent to the clients
*/

#pragma pack(push, 1)
// this will be sent to the client
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

    // 16 bit -> 2 byte
    //uint16_t game_status; //1 = running, else = not

}data;

// this will be received from the client
typedef struct player_mv{ // struct for player movement
    uint8_t player_status;
    uint8_t player_w;
    uint8_t player_s;
}mv;
#pragma pack(pop)


void reset(struct game_data*); // reset the game status
float time_now_sec(); // return the current time in seconds
void run(struct game_data*); // deprecated
void update(struct game_data*, const unsigned int, const unsigned int, const unsigned int, const unsigned int, float,int*); // updates the game status
int pack_data(struct game_data*, unsigned char*, size_t); // process the data in order to send it to the client
void unpack_data(struct player_mv*, const unsigned char*); // process the packet in order to read the data from the client

#endif 