#include "../../includes/clnt.h"

/*

1280x800 resolution

required data:

player 1 x, y uint16_t
player 2 x, y uint16_t
player 1 score uint16_t
player 2 score uint16_t
ball x uint16_t
ball y uint16_t
stop game uint16_t
fin zeros uint16_t

data_size = uint16_t

*/


int init_channel(struct one_way_channel* owc){
    owc->data = (data_size)malloc(sizeof(data_size) * PACKET_SIZE);
    memset(owc->data, 0, sizeof(data_size) * PACKET_SIZE);
    owc->has_data = 0;

    pthread_mutex_init(&owc->mutex, NULL);
    pthread_cond_init(&owc->cond, NULL);
} // initialize one way mutex

int init_twoway(struct two_way_channel* twc){
    init_channel(&twc->command);
    init_channel(&twc->response);
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

} // client work

void channel_send(struct one_way_channel* owc, const data_size full_data[PACKET_SIZE]){
    pthread_mutex_lock(&owc->mutex);
    while(owc->has_data) {
        pthread_cond_wait(&owc->cond, &owc->mutex);
    }
    memcpy(owc->data, full_data, sizeof(data_size) * PACKET_SIZE);
    owc->has_data = 1;
    pthread_cond_signal(&owc->cond);
    pthread_mutex_unlock(&owc->mutex);
} // send data to the parent process

void channel_receive(struct one_way_channel* owc, data_size buffer[PACKET_SIZE]){
    pthread_mutex_lock(&owc->mutex);
    while(!owc->has_data) {
        pthread_cond_wait(&owc->cond, &owc->mutex);
    }

    memcpy(buffer, owc->data, PACKET_SIZE);
    owc->has_data = 0;

    pthread_cond_signal(&owc->cond);
    pthread_mutex_unlock(&owc->mutex);

} // receive data from the parent process

void close_twoway(struct two_way_channel* channel) {

}

void close_channel(struct one_way_channel* channel) {

}