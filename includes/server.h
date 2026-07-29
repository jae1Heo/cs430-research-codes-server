#ifndef __SERVER_H__
#define __SERVER_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

enum {
    GAME_WAIT = 0,
    GAME_HANDSHAKE,
    GAME_RUNNING
};

typedef struct sock{ // socket structure 
    int sock_fd;
    struct sockaddr_in sock_addr;
    socklen_t sock_len;
}Sock;

int init_server(struct sock*, const unsigned short); 
int connect_client(struct sock*, struct sock*);
int send_all(struct sock*, const void*, size_t);
int recv_all(struct sock*, void*, size_t);
int send_packet(struct sock*, const void*, uint16_t);
int recv_packet(struct sock*, void*, uint16_t*);
void close_sock(struct sock*);


#endif 