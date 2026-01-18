#include "../includes/clnt.h"

#define PORT 12345

int main(int argc, char* argv[]) {
    
    struct sock server_sock;
    struct sock client_socks[MAX_CLIENTS];
    pthread_t client_threads[MAX_CLIENTS];
    struct two_way_channel* channels[MAX_CLIENTS];
    struct client_info thread_args[MAX_CLIENTS];
    int is_started = 0;

    // Hardcoded keys and IVs for 2 clients
    unsigned char hardcoded_keys[MAX_CLIENTS][AES_KEY_SIZE] = {
        {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
         0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F},
        {0xFF,0xFE,0xFD,0xFC,0xFB,0xFA,0xF9,0xF8,0xF7,0xF6,0xF5,0xF4,0xF3,0xF2,0xF1,0xF0,
         0xEF,0xEE,0xED,0xEC,0xEB,0xEA,0xE9,0xE8,0xE7,0xE6,0xE5,0xE4,0xE3,0xE2,0xE1,0xE0}
    };

    unsigned char hardcoded_ivs[MAX_CLIENTS][AES_GCM_IV_SIZE] = {
        {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B},
        {0xFF,0xFE,0xFD,0xFC,0xFB,0xFA,0xF9,0xF8,0xF7,0xF6,0xF5,0xF4}
    };

    if(!init_server(&server_sock, htons(PORT))) {
        fputs("init_server() error\n", stderr);
        exit(1);
    }

    for(int i = 0; i < MAX_CLIENTS; i++) {
        channels[i] = malloc(sizeof(struct two_way_channel));
        if(!init_twoway(channels[i])) {
            fputs("init_twoway() failed\n", stderr);
            exit(1);
        }

        if(!connect_client(&server_sock, &client_socks[i])) {
            fputs("connect_client() error\n", stderr);
            exit(1);
        }

        thread_args[i].clinet_id = i + 1;
        thread_args[i].clnt_sock = client_socks[i];
        thread_args[i].channel = channels[i];

        // Use hardcoded keys and IVs instead of generating
        memcpy(thread_args[i].key, hardcoded_keys[i], AES_KEY_SIZE);
        memcpy(thread_args[i].iv, hardcoded_ivs[i], AES_GCM_IV_SIZE);

        pthread_create(&client_threads[i], NULL, client_main, &thread_args[i]);

        // here, receive packet from the client 
    }

    // Main game loop
    while(1) {
        packet_data client_data[MAX_CLIENTS][PACKET_SIZE];

        // Receive from clients
        for(int i = 0; i < MAX_CLIENTS; i++) {
            channel_receive(&channels[i]->command, client_data[i]);
        }


        // clients will send 1 and if there are two 1, then start the game.
        // first, client will send "0" after connection.
        // server will then send the initial game data
        // when both client is there, the server will updated game data
        
        if(!is_started) {
            if(client_data[0][0] == 1 && client_data[1][0] == 1) {
                is_started = 1;
            }
        }
        else {
            // Process game state
            pack_data(client_data[0], client_data[1]);
            
            // Send updated state to clients
            for(int i = 0; i < MAX_CLIENTS; i++) {
                channel_send(&channels[i]->response, client_data[i]);
            }
        }

    }

    // Cleanup (never reached in current infinite loop)
    for(int i = 0; i < MAX_CLIENTS; i++) {
        pthread_join(client_threads[i], NULL);
        close_twoway(channels[i]);
    }

    return 0;
}
