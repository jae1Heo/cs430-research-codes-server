#include "game.h"


void pack_data(packet_data buffer_player_1[PACKET_SIZE], packet_data buffer_player_2[PACKET_SIZE]) {
    update_score(buffer_player_1, buffer_player_2);
    sync_movement(buffer_player_1, buffer_player_2);
}

packet_data validate_data(packet_data from_clnt1, packet_data from_clnt2) {
    if(from_clnt1 == from_clnt2) {
        return from_clnt1;
    }
    else {
        return -1;
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
    packet_data ball_pos_y = validate_data(buffer_player_1[ball_y], buffer_player_2[ball_y]);

    int score_flag =  calculate_score(ball_pos_x, ball_pos_y);

    if(score_flag < 0) {
        return;
    }

    if(score_flag == 1) { // player 2 win
        buffer_player_2[player1_score]++;
        buffer_player_1[player2_score]++;
    }
    else if(score_flag == 0) { // player 1 win
        buffer_player_1[player1_score]++;
        buffer_player_2[player2_score]++;
    }
}

void sync_movement(packet_data buffer_player_1[PACKET_SIZE], packet_data buffer_player_2[PACKET_SIZE]) {

}