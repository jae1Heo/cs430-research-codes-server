#ifndef __GAME_H__
#define __GAME_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>


#define ORIGIN 0
#define WIDTH 1280
#define HEIGHT 800
#define PACKET_INVALID 0xFFFF

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT = 800

#define  BALL_SPEED 150.0f
#define PADDLE_SPEED 300.0f
#define PADDLE_WIDTH 15.0f
#define PADDLE_HEIGHT 90.0f
#define BALL_SIZE 15.0f

/*
game data will be packed as packet_data variable and sent to the clients
*/

enum {player1_x = 0, player1_y, player2_x, player2_y, player1_score, player2_score, ball_x, ball_y, stop_game, reserved};
enum {player1_w = 0, player1_s, player2_w, player2_s};




#define RAW_PACKET_SIZE 9
#define PACKET_SIZE 4
typedef uint16_t packet_data;

void pack_data(packet_data[PACKET_SIZE], packet_data[PACKET_SIZE]);


#endif 