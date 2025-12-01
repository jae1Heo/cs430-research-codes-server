#include "../includes/clnt.h"

#define PORT 12345

int main(int argc, char* argv[]) {


    struct sock server_sock;
    struct sock client_socks[MAX_CLIENTS];
    pthread_t client_threads[MAX_CLIENTS];
    struct two_way_channel* channels[MAX_CLIENTS];
    struct client_info thread_args[MAX_CLIENTS];

    if(!init_server(&server_sock, PORT)) {
        fputs("init_server() error", stderr);
        exit(1);
    }

    for(int i = 0; i < MAX_CLIENTS; i++) {
        channels[i] = malloc(sizeof(struct two_way_channel));
        if(!init_twoway(channels[i])) {
            fputs("init_twoway() failed", stderr);
            exit(1);
        }

        if(!connect_client(&server_sock, &client_socks[i])) {
            fputs("connect_client() error", stderr);
            exit(1);
        }

        thread_args[i].clinet_id = i + 1;
        thread_args[i].clnt_sock = client_socks[i];
        thread_args[i].channel = channels[i];

        generate_symmetric_key(thread_args[i].key, AES_KEY_SIZE);
        generate_iv(thread_args[i].iv, AES_GCM_IV_SIZE);

        pthread_create(&client_threads[i], NULL, client_main, &thread_args[i]);

    }

    while(1) {
        packet_data client_data[MAX_CLIENTS][PACKET_SIZE];

        for(int i = 0; i < MAX_CLIENTS; i++) {
            channel_receive(&channels[i]->command, client_data[i]);
        }

        pack_data(client_data[0], client_data[1]);

        for(int i = 0; i < MAX_CLIENTS; i++) {
            channel_send(&channels[i]->response, client_data[i]);
        }
    }

    for(int i = 0; i < MAX_CLIENTS; i++) {
        pthread_join(client_threads[i], NULL);
        close_twoway(channels[i]);
    }

    return 0;
}