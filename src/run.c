#include "../includes/clnt.h"

#define PORT 12345


int main(int argc, char* argv[]) {
    // variables initialization
    srand(time(NULL)); 
    struct sock server_sock; // server socket
    struct sock client_socks[MAX_CLIENTS]; // client sockets
    pthread_t client_threads[MAX_CLIENTS]; // threads for each client socket
    struct two_way_channel* channels[MAX_CLIENTS]; // mutex channels for client - server communication
    struct client_info thread_args[MAX_CLIENTS]; // thread information for each client
    int game_status = 0; // game status
    int running_status = GAME_WAIT; // running status

    // initializing server socket
    if(!init_server(&server_sock, htons(PORT))) {
        fputs("init_server() error\n", stderr);
        exit(1);
    }

    // waiting for each client to connect
    for(int i = 0; i < MAX_CLIENTS; i++) {
        channels[i] = malloc(sizeof(struct two_way_channel)); 
        // initializing mutex channel for each client
        if(!init_twoway(channels[i])) { 
            fputs("init_twoway() failed\n", stderr);
            exit(1);
        }

        // here, waiting for client to connect
        // server will wait until client to connect
        if(!connect_client(&server_sock, &client_socks[i])) {
            fputs("connect_client() error\n", stderr);
            exit(1);
        }

        // initializing thread for each client
        thread_args[i].client_id = i + 1;
        thread_args[i].clnt_sock = client_socks[i];
        thread_args[i].channel = channels[i];

        // create thread for connected client
        pthread_create(&client_threads[i], NULL, client_main, &thread_args[i]);

        // here, receive packet from the client 
    }

    // initializng game data
    struct game_data g_data;
    memset(&g_data, 0, sizeof(g_data));

    struct player_mv left_data;  // side = 1
    struct player_mv right_data; // side = 2

    // initializng clock cycle
    float last_time = time_now_sec();
    unsigned long long tick = 0;
    float now = 0.0f;
    float frame_time = 0.0f;

    // initializng packet container for each client
    unsigned char client_data[MAX_CLIENTS][PACKET_MAX];
    int init_flag = 1;
    
    while(1) {
        now = time_now_sec();
        frame_time = now - last_time;
        last_time = now;

        // receive from clients
        for(int i = 0; i < MAX_CLIENTS; i++) {
            channel_receive(&channels[i]->command, client_data[i]);
        }
        
        //game status will be GAME_WAIT until two clients sends ready sign

        if(running_status == GAME_WAIT) {
            // clients will send 'j' as ready sign
            if(client_data[0][0] == 'j' && client_data[1][0] == 'j') {
                // reset() x reset the score
                // g_data reset before the game starts
                //g_data.game_status = 1;

                // assign side to each clients
                for(int i = 0; i < MAX_CLIENTS; i++) {
                    
                    //variable side will determine which side for each client to play
                    //1 will be left, and 2 will be right
                    
                    uint8_t side = i + 1;
                    memset(client_data[i], 0, PACKET_MAX);
                    client_data[i][0] = 's';
                    client_data[i][1] = side;
                }
                
                //there will be a transition in game state
                //after each client receive packet with 's', each client will intiate handshaking
                
                running_status = GAME_HANDSHAKE;
            }
        }
        else if(running_status == GAME_HANDSHAKE) {
            // when both client is ready for handshaking, will send pakcet with 'a'
            if(client_data[0][0] == 'a' && client_data[1][0] == 'a') {
                reset(&g_data); 
                // pack updated data (init)
                for(int i = 0; i < MAX_CLIENTS; i++) {
                    pack_data(&g_data, client_data[i], PACKET_MAX);
                }
                running_status = GAME_RUNNING;
            }
        }
        else {
            if(client_data[0][0] == 'p' && client_data[1][0] == 'p') {
                // in game 
                game_status = 1; // set the game status to 1
                // think I should move these out of the loop, and memset()
                // deprecated. 
                //struct player_mv left_data;  // side = 1
                //struct player_mv right_data; // side = 2

                if(init_flag) {
                    init_flag = false;
                }
                else {
                    unpack_data(&left_data, client_data[0]); // unpack the data from left side client
                    unpack_data(&right_data, client_data[1]); // unpack the data from right side client
                
                    // update the game status as received packet from each client
                    update(&g_data, left_data.player_w, left_data.player_s, right_data.player_w, right_data.player_s, frame_time, &game_status);
                }

                // here, update() will set the game_status to 0 if game is set or one client made a point
                if(!game_status) {
                    reset(&g_data);
                }

                // pack the updated data as formatted packet
                for(int i = 0; i < MAX_CLIENTS; i++) {
                    pack_data(&g_data, client_data[i], PACKET_MAX);
                }

            }
        }

        // process game state
        // otherwise receive, [1 = w keyup 0 = down] [1 = s keyup 0 = down] [] [] 
        // update client info with client data
        // then send, left_y, right_y, ball_x, ball_y, score_left, score_right, game_status

        // if game_status = reset -> call reset() and send back the game data to clients
        // send updated state to clients
        for(int i = 0; i < MAX_CLIENTS; i++) {
            channel_send(&channels[i]->response, client_data[i]);
        }

        memset((void*)&left_data, 0, sizeof(struct player_mv));
        memset((void*)&right_data, 0, sizeof(struct player_mv));
        for(int i = 0; i < MAX_CLIENTS; i++) {
            memset((void*)client_data[i], 0, PACKET_MAX);
        }

        usleep(8333); 
        // formula
        // T = Time period for one frame in second(s)
        // f = Frequency in Hertz
        // T = 1/f
        // f = 1/T
        // T = 1/120 = 0.008333~ so 8.333 milliseconds will catch up 120 FPS. 
        
    }
    

    // cleanup (never reached in current infinite loop)
    /*
    clients will send termination sign to server, and server will execute this
    upadte required
    */
    for(int i = 0; i < MAX_CLIENTS; i++) {
        pthread_join(client_threads[i], NULL);
        close_twoway(channels[i]);
        close_sock(&client_socks[i]);
    }

    return 0;
}
