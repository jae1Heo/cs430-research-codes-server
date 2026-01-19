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
typedef uint16_t packet_data;

/*
game data will be packed as packet_data variable and sent to the clients
*/


typedef struct game_data{
    int left_score;
    int right_score;

    float ball_pos_x;
    float ball_pos_y;
    float ball_vel_x;
    float ball_vel_y;

    float left_paddle_y;
    float right_paddle_y;

    unsigned int game_status; //1 = running, else = not

}data;




void reset(struct game_data*);
double time_now_sec();
void run(struct game_data*);
void update(struct game_data*, const unsigned int, const unsigned int, const unsigned int, const unsigned int, double);


#endif 