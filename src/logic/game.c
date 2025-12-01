#include "../../includes/game.h"

void pack_data(packet_data buffer_player_1[PACKET_SIZE], packet_data buffer_player_2[PACKET_SIZE]) {
    update_score(buffer_player_1, buffer_player_2);
    sync_movement(buffer_player_1, buffer_player_2);
}

packet_data validate_data(packet_data from_clnt1, packet_data from_clnt2) {
    if(from_clnt1 == from_clnt2) {
        return from_clnt1;
    }
    else {
        return PACKET_INVALID;
    }
}

const int calculate_score(packet_data ball_pos_x, packet_data ball_pos_y) {
    /*
    (0, 0), (0, 800) -> player 2 point
    (1280, 0), (1280, 800) -> player 1 point

    0 if player 1 point
    1 if player 2 point
    */

    if(ball_pos_x <= 0) {
        return 1;
    }
    else if(ball_pos_x >= 1280) {
        return 0;
    }
    else {
        return -1;
    }

}

void update_score(packet_data buffer_player_1[PACKET_SIZE], packet_data buffer_player_2[PACKET_SIZE]) {

    packet_data ball_pos_x = validate_data(buffer_player_1[ball_x], buffer_player_2[ball_x]);

    if(ball_pos_x == PACKET_INVALID) {
        return;
    }
    
    packet_data ball_pos_y = validate_data(buffer_player_1[ball_y], buffer_player_2[ball_y]);
    if(ball_pos_y == PACKET_INVALID) {
        return;
    }

    int score_flag =  calculate_score(ball_pos_x, ball_pos_y);

    if(score_flag < 0) {
        return;
    }

    if(score_flag == 1) { // player 2 win
        buffer_player_2[player2_score]++;
        buffer_player_1[player2_score]++;
    }
    else if(score_flag == 0) { // player 1 win
        buffer_player_1[player1_score]++;
        buffer_player_2[player1_score]++;
    }
}

void sync_movement(packet_data buffer_player_1[PACKET_SIZE], packet_data buffer_player_2[PACKET_SIZE]) {
    packet_data player1_pos_x = validate_data(buffer_player_1[player1_x], buffer_player_2[player1_x]);
    packet_data player1_pos_y = validate_data(buffer_player_1[player1_y], buffer_player_2[player1_y]);
    packet_data player2_pos_x = validate_data(buffer_player_1[player2_x], buffer_player_2[player2_x]);
    packet_data player2_pos_y = validate_data(buffer_player_1[player2_y], buffer_player_2[player2_y]);
    packet_data ball_pos_x = validate_data(buffer_player_1[ball_x], buffer_player_2[ball_x]);
    packet_data ball_pos_y = validate_data(buffer_player_1[ball_y], buffer_player_2[ball_y]);

   if(player1_pos_x != PACKET_INVALID) {
        buffer_player_1[player1_x] = player1_pos_x;
        buffer_player_2[player1_x] = player1_pos_x;
    }

    if(player1_pos_y != PACKET_INVALID) {
        buffer_player_1[player1_y] = player1_pos_y;
        buffer_player_2[player1_y] = player1_pos_y;
    }

    if(player2_pos_x != PACKET_INVALID) {
        buffer_player_1[player2_x] = player2_pos_x;
        buffer_player_2[player2_x] = player2_pos_x;
    }

    if(player2_pos_y != PACKET_INVALID) {
        buffer_player_1[player2_y] = player2_pos_y;
        buffer_player_2[player2_y] = player2_pos_y;
    }

    if(ball_pos_x != PACKET_INVALID) {
        buffer_player_1[ball_x] = ball_pos_x;
        buffer_player_2[ball_x] = ball_pos_x;
    }

    if(ball_pos_y != PACKET_INVALID) {
        buffer_player_1[ball_y] = ball_pos_y;
        buffer_player_2[ball_y] = ball_pos_y;
    }
}