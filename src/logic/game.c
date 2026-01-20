#include "../../includes/game.h"



void reset(struct game_data* g_data) {
    srand(time(NULL));
    
    g_data->ball_pos_x = (WINDOW_WIDTH / 2)  - (BALL_SIZE / 2);
    g_data->ball_pos_y = (WINDOW_HEIGHT / 2) - (BALL_SIZE / 2);
    
    g_data->ball_vel_x = (rand() % 2 == 0) ? BALL_SPEED : -BALL_SPEED;
    g_data->ball_vel_y = (rand() % 2 == 0) ? BALL_SPEED : -BALL_SPEED;

    g_data->left_paddle_y = (WINDOW_HEIGHT / 2) - (PADDLE_HEIGHT / 2);
    g_data->right_paddle_y = (WINDOW_HEIGHT / 2) - (PADDLE_HEIGHT / 2);
}

double time_now_sec() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
}

void update(struct game_data* g_data, const unsigned int w_key_left, const unsigned int s_key_left, const unsigned int w_key_right, const unsigned int s_key_right, double delta) {
    // left side update
    if(w_key_left &&  g_data->left_paddle_y > 0) {
        g_data->left_paddle_y -= PADDLE_SPEED * delta;
    }
    else if(s_key_left && g_data->left_paddle_y < WINDOW_HEIGHT - PADDLE_HEIGHT) {
        g_data->left_paddle_y += PADDLE_SPEED * delta;
    } 

    // right side update
    if(w_key_right && g_data->right_paddle_y > 0) {
        g_data->right_paddle_y -= PADDLE_SPEED * delta;
    }
    else if(s_key_right && g_data->right_paddle_y < WINDOW_HEIGHT - PADDLE_HEIGHT) {
        g_data->right_paddle_y += PADDLE_SPEED * delta;
    }

    // ball position update
    g_data->ball_pos_x += g_data->ball_vel_x * delta;
    g_data->ball_pos_y += g_data->ball_vel_y * delta;

    // ball bounce check
    if(g_data->ball_pos_y <= 0 || g_data->ball_pos_y + BALL_SIZE >= WINDOW_HEIGHT) {
        g_data->ball_vel_y = g_data->ball_vel_y * -1;
    }


    // check if ball overlaps the paddles vertically
    unsigned int check_if_ball_in_left_range_y = 0;
    unsigned int check_if_ball_in_right_range_y = 0;

    if(g_data->ball_pos_y + BALL_SIZE >= g_data->left_paddle_y) {
        if(g_data->ball_pos_y <= g_data->left_paddle_y + PADDLE_HEIGHT) {
            check_if_ball_in_left_range_y = 1;
        }
    }

    if(g_data->ball_pos_y + BALL_SIZE >= g_data->right_paddle_y) {
        if(g_data->ball_pos_y <= g_data->right_paddle_y + PADDLE_HEIGHT) {
            check_if_ball_in_right_range_y = 1;
        }
    }

    // checks if ball overlaps the paddles horizontally + vertically
    if(g_data->ball_pos_x <= 20 + PADDLE_WIDTH && g_data->ball_pos_x >= 20 && check_if_ball_in_left_range_y) {
        g_data->ball_pos_x = 20 + PADDLE_WIDTH;
        g_data->ball_vel_x = g_data->ball_vel_x * -1;
    }

    if(g_data->ball_pos_x + BALL_SIZE >= WINDOW_WIDTH - 20 - PADDLE_WIDTH && g_data->ball_pos_x <= WINDOW_WIDTH - 20 && check_if_ball_in_right_range_y) {
        g_data->ball_pos_x = WINDOW_WIDTH - 20 - PADDLE_WIDTH - BALL_SIZE;
        g_data->ball_vel_x = g_data->ball_vel_x * -1;
    }


    // check score 
    if(g_data->ball_pos_x <= 0) {
        g_data->right_score++;
        g_data->game_status = 0;
    }

    if(g_data->ball_pos_x + BALL_SIZE >= WINDOW_WIDTH) {
        g_data->left_score++;
        g_data->game_status = 0;
    }


}

int pack_data(struct game_data* g_data, char* buffer, size_t buffer_len) {
    if(buffer_len < sizeof(*g_data)) {
        fputs("pack limit exceeded", stderr);
        return 0;
    }
    memcpy(buffer, g_data, sizeof(*g_data));
    return 1;
}

void unpack_data(struct player_mv* p_data, const unsigned char* buffer) {
    size_t offset = 0;
    p_data->player_w = *(uint8_t*)(buffer + offset);
    offset += sizeof(uint8_t);
    p_data->player_s = *(uint8_t*)(buffer + offset);
    offset += sizeof(uint8_t);
}