#include "../../includes/clnt.h"


// initialize mutex channel
int init_channel(struct one_way_channel* owc){
    owc->data = malloc(sizeof(unsigned char) * PACKET_MAX);

    if(owc->data == NULL) {
        return 0;
    }

    memset(owc->data, 0, sizeof(unsigned char) * PACKET_MAX);
    owc->has_data = 0;

    pthread_mutex_init(&owc->mutex, NULL);
    pthread_cond_init(&owc->cond, NULL);
    
    return 1;
} 

// with the created one way mutex channel, create a bi-directional mutex channel between client and server
int init_twoway(struct two_way_channel* twc){
    
    if(!init_channel(&twc->command)) {
        perror("init_channel() command error");
        return 0;
    }
    if(!init_channel(&twc->response)) {
        perror("init_channel() response error");
        return 0;
    }

    return 1;
} // initialize two way mutex

/*

void* client_main(void *arg) {
    two_way_channel_t *tw = arg;

    while (1) {

        // Wait for command from main
        uint64_t cmd = channel_receive(&tw->command);
        printf("Worker: received command %d\n", cmd);

        it should be like:

                            main process
                                 ^
                                / \
                               /   \
                              /     \
                             /       \
                        thread 1    thread 2
            
            1. each thread receive data from the client
            2. each thread dehashes data from the client
            2. each thread send data to main process
            3. main process unpack data from threads and pack it back
            4. each thread hash the data
            5. each thread send back the hashed data to the client
        
        

        // Process and send response
        data_size result = pack(<data>);
        channel_send(&tw->response, result);
    }

    return NULL;
}

*/

void* client_main(void* args){
    
    // initialize variables 
    struct client_info* client = (struct client_info*)args; // initialize client info with argument from main thread 
    unsigned char recv_buffer[PACKET_MAX]; // initialize recv buffer 
    unsigned char send_buffer[PACKET_MAX]; // initialize send buffer
    unsigned char plaintext[PACKET_MAX];
    unsigned char ciphertext[PACKET_MAX];
    uint16_t p_len; // length of the packet

    memset(recv_buffer, 0, PACKET_MAX);
    memset(send_buffer, 0, PACKET_MAX);
    memset(plaintext, 0, PACKET_MAX);
    memset(ciphertext, 0, PACKET_MAX);

    while(1) {

        if(!recv_packet(&client->clnt_sock, recv_buffer, &p_len)) {
            fputs("client disconnected", stderr);
            break;
        }

        envelope env;
        memset(&env, 0, sizeof(envelope));
        memcpy(&env, recv_buffer, sizeof(envelope));

        if(!resolve_envelope((const unsigned char*)&env, sizeof(env), plaintext)) {
            fputs("failed to decrypt packet\n", stderr);
            break;
        }

        channel_send(&client->channel->command, plaintext);

        channel_receive(&client->channel->response, send_buffer);

        memset(&env, 0, sizeof(envelope));
        if(!build_envelope(&env, sizeof(struct game_data), send_buffer)) {
            fputs("failed to encrypt packet\n", stderr);
            break;
        }

        if(!send_packet(&client->clnt_sock, &env, (uint16_t)sizeof(envelope))) {
            fputs("failed to send packet to client", stderr);
            break;
        }


        memset(send_buffer, 0, PACKET_MAX);
        memset(recv_buffer, 0, PACKET_MAX);
        memset(plaintext, 0, PACKET_MAX);
        memset(ciphertext, 0, PACKET_MAX);
        p_len = 0;
        
        /*
        if(!recv_packet(&client->clnt_sock, recv_buffer, &p_len)) {
            fputs("client disconnected", stderr);
            break;
        }

        if(!aes_decrypt(recv_buffer, p_len, plaintext)) {
            fputs("packet decryption failed\n", stderr);
            break;
        }
        
        // send data to main process
        channel_send(&client->channel->command, recv_buffer);
        // rreceive data from the main process
        channel_receive(&client->channel->response, send_buffer);

        int cipherlen = aes_encrypt(send_buffer, sizeof(struct game_data), ciphertext);
        if(cipherlen < 0) {
            fputs("failed to encrypt packet\n", stderr);
            break;
        }

        if(!send_packet(&client->clnt_sock, send_buffer, cipherlen)) {
            fputs("failed to send to client", stderr);
            break;
        }


        memset(recv_buffer, 0, PACKET_MAX);
        memset(send_buffer, 0, PACKET_MAX);
        memset(plaintext, 0, PACKET_MAX);
        memset(ciphertext, 0, PACKET_MAX);
        */
    }

    return NULL;

} // client work

// send the data to the main process
void channel_send(struct one_way_channel* owc, const unsigned char full_data[PACKET_MAX]){
    pthread_mutex_lock(&owc->mutex); // lock the mutex
    while(owc->has_data) { // if the mutex buffer has data init
        pthread_cond_wait(&owc->cond, &owc->mutex); // suspend the thread
    }
    memcpy(owc->data, full_data, sizeof(unsigned char) * PACKET_MAX); // copy the data from mutex buffer to the store buffer
    owc->has_data = 1; // set the flag that thread received data
    pthread_cond_signal(&owc->cond); 
    pthread_mutex_unlock(&owc->mutex); // unlock the mutex channel
} 

// receive the data from the main process
void channel_receive(struct one_way_channel* owc, unsigned char buffer[PACKET_MAX]){
    pthread_mutex_lock(&owc->mutex); // lock the mutex
    while(!owc->has_data) { // loop until mutex has data to send
        pthread_cond_wait(&owc->cond, &owc->mutex); // suspend the thread
    }

    memcpy(buffer, owc->data, sizeof(unsigned char) * PACKET_MAX); // copy the data from store buffer to mutex buffer
    owc->has_data = 0; // set the flag that mutex released the data

    pthread_cond_signal(&owc->cond);
    pthread_mutex_unlock(&owc->mutex); // unlock the mutex channel

} 

// close the mutex channel (two-way channel)
// wrapper function of close_channel()
void close_twoway(struct two_way_channel* channel) {
    if(channel == NULL) {
        return;
    }

    close_channel(&channel->command);
    close_channel(&channel->response);
    free(channel);
}

// close the mutex channel (one-way channel)
void close_channel(struct one_way_channel* channel) {

    if(channel == NULL) {
        return;
    }
    free((void*)channel->data);
    channel->data = NULL;

    pthread_mutex_destroy(&channel->mutex);
    pthread_cond_destroy(&channel->cond);
}