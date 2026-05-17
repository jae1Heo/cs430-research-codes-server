#include "../../includes/game.h"

// reset the game status
void reset(struct game_data* g_data) {
    // set the position of the ball at the center of the screen
    g_data->ball_pos_x = (WINDOW_WIDTH / 2.0f)  - (BALL_SIZE / 2.0f);
    g_data->ball_pos_y = (WINDOW_HEIGHT / 2.0f) - (BALL_SIZE / 2.0f);
    
    // randomize the initial speed of the ball
    g_data->ball_vel_x = (rand() % 2 == 0) ? BALL_SPEED : -BALL_SPEED;
    g_data->ball_vel_y = (rand() % 2 == 0) ? BALL_SPEED : -BALL_SPEED;

    // reset the location of paddles
    g_data->left_paddle_y = (WINDOW_HEIGHT / 2.0f) - (PADDLE_HEIGHT / 2.0f);
    g_data->right_paddle_y = (WINDOW_HEIGHT / 2.0f) - (PADDLE_HEIGHT / 2.0f);

    g_data->left_paddle_x = PADDLE_WIDTH_MARGIN;
    g_data->right_paddle_x = WINDOW_WIDTH - PADDLE_WIDTH_MARGIN - PADDLE_WIDTH;
}

// returns current time in seconds
float time_now_sec() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (float)ts.tv_sec + (float)ts.tv_nsec * 1e-9f;
}

// update the game status
void update(struct game_data* g_data, const unsigned int w_key_left, const unsigned int s_key_left, const unsigned int w_key_right, const unsigned int s_key_right, float delta, int* game_status) {
    // ball position update
    g_data->ball_pos_x += g_data->ball_vel_x * delta;
    g_data->ball_pos_y += g_data->ball_vel_y * delta;

    // check if ball overlaps the paddles vertically
    // flags that checks if the ball's position is overlapping with the 
    unsigned int check_if_ball_in_left_range_y = 0;
    unsigned int check_if_ball_in_right_range_y = 0;

    // if the ball's position is overlapping with the left side paddle, then adjust the ball's position
    if(g_data->ball_pos_y + BALL_SIZE >= g_data->left_paddle_y) {
        if(g_data->ball_pos_y <= g_data->left_paddle_y + PADDLE_HEIGHT) {
            check_if_ball_in_left_range_y = 1;
        }
    }

    // if the ball's position is overlapping with the right side paddle, then adjust the ball's position
    if(g_data->ball_pos_y + BALL_SIZE >= g_data->right_paddle_y) {
        if(g_data->ball_pos_y <= g_data->right_paddle_y + PADDLE_HEIGHT) {
            check_if_ball_in_right_range_y = 1;
        }
    }

    // checks if ball overlaps the paddles horizontally + vertically
    // left side
    if(g_data->ball_pos_x <= 20 + PADDLE_WIDTH && g_data->ball_pos_x >= 20 && check_if_ball_in_left_range_y) {
        g_data->ball_pos_x = 20 + PADDLE_WIDTH;
        g_data->ball_vel_x = g_data->ball_vel_x * -1;
    }

    // checks if ball overlaps the paddle horizontally + vertically
    // right side
    if(g_data->ball_pos_x + BALL_SIZE >= WINDOW_WIDTH - 20 - PADDLE_WIDTH && g_data->ball_pos_x <= WINDOW_WIDTH - 20 && check_if_ball_in_right_range_y) {
        g_data->ball_pos_x = WINDOW_WIDTH - 20 - PADDLE_WIDTH - BALL_SIZE;
        g_data->ball_vel_x = g_data->ball_vel_x * -1;
    }

    // check if ball overlaps the window's boundaries (vertically)
    if(g_data->ball_pos_y <= 0 || g_data->ball_pos_y + BALL_SIZE >= WINDOW_HEIGHT) {
        g_data->ball_vel_y = g_data->ball_vel_y * -1;
    }

    // left side update
    // if the left side w key is pressed, move the left side paddle upward
    if(w_key_left &&  g_data->left_paddle_y > 0) {
        g_data->left_paddle_y -= PADDLE_SPEED * delta;
    }
    // if the left side s key is pressed, move the left side paddle downward
    else if(s_key_left && g_data->left_paddle_y < WINDOW_HEIGHT - PADDLE_HEIGHT) {
        g_data->left_paddle_y += PADDLE_SPEED * delta;
    } 

    // right side update
    // if the right side w key is pressed, move the right side paddle upward
    if(w_key_right && g_data->right_paddle_y > 0) {
        g_data->right_paddle_y -= PADDLE_SPEED * delta;
    }
    // if the right side s key is pressed, move the right side paddle downward
    else if(s_key_right && g_data->right_paddle_y < WINDOW_HEIGHT - PADDLE_HEIGHT) {
        g_data->right_paddle_y += PADDLE_SPEED * delta;
    }

    // check score 
    // checks if the right side made a score
    if(g_data->ball_pos_x <= 0) {
        g_data->right_score++;
        *game_status = 0;
    }

    // checks if the left side made a score
    if(g_data->ball_pos_x + BALL_SIZE >= WINDOW_WIDTH) {
        g_data->left_score++;
        *game_status = 0;
    }

}

// pack the data into packet
int pack_data(struct game_data* g_data, unsigned char* buffer, size_t buffer_len) {
    if(buffer_len < sizeof(*g_data)) {
        fputs("pack limit exceeded", stderr);
        return 0;
    }
    memcpy(buffer, g_data, sizeof(*g_data));
    return 1;
}

// unpack the packet into readable data
void unpack_data(struct player_mv* p_data, const unsigned char* buffer) {
    size_t offset = 0;
    p_data->player_status = *(uint8_t*)(buffer + offset);
    offset += sizeof(uint8_t);
    p_data->player_w = *(uint8_t*)(buffer + offset);
    offset += sizeof(uint8_t);
    p_data->player_s = *(uint8_t*)(buffer + offset);
    offset += sizeof(uint8_t);
}