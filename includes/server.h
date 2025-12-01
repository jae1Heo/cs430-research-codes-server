#ifndef __SERVER_H__
#define __SERVER_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

typedef struct sock{
    int sock_fd;
    struct sockaddr_in sock_addr;
    socklen_t sock_len;
}Sock;

int init_server(struct sock*, const unsigned short);
int connect_client(struct sock*, struct sock*);



#endif 