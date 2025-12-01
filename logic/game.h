#ifndef __GAME_H__
#define __GAME_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>


#define ORIGIN 0
#define WIDTH 1280
#define HEIGHT 800

/*
game data will be packed as packet_data variable and sent to the clients
*/

enum {player1_x = 0, player1_y, player2_x, player2_y, player1_score, player2_score, ball_x, ball_y, stop_game, reserved};

#define PACKET_SIZE 8
typedef uint16_t packet_data;

void pack_data(packet_data[PACKET_SIZE], packet_data[PACKET_SIZE]);
void update_score(packet_data[PACKET_SIZE], packet_data[PACKET_SIZE]);
const int calculate_score(packet_data, packet_data);
packet_data validate_data(packet_data, packet_data);
void sync_movement(packet_data[PACKET_SIZE], packet_data[PACKET_SIZE]);

#endif 