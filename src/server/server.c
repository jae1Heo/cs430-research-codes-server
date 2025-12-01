#include "../../includes/server.h"


// port should be unsigned short and have to be converted into network byte order
int init_server(struct sock* serv_sock, const unsigned short un_port) {
    serv_sock->sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(serv_sock->sock_fd < 0) {
        perror("socket() error");
        return 0;
    }

    memset((void*)&serv_sock->sock_addr, 0, sizeof(serv_sock->sock_addr));

    serv_sock->sock_addr.sin_family = AF_INET;
    serv_sock->sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_sock->sock_addr.sin_port = un_port;

    serv_sock->sock_len = sizeof(serv_sock->sock_addr);

    if(bind(serv_sock->sock_fd, (struct sockaddr*)&serv_sock->sock_addr, serv_sock->sock_len) < 0) {
        perror("bind() error");
        return 0;
    }

    if(listen(serv_sock->sock_fd, 3) < 0) {
        perror("listen() error");
        return 0;
    }

    return 1;
}


// called by client thread
int connect_client(struct sock* serv_sock, struct sock* clnt_sock) {
    if(serv_sock == NULL) {
        fputs("server socket not initialized", stderr);
        return 0;
    }

    memset((void*)&clnt_sock->sock_addr, 0, sizeof(clnt_sock->sock_addr));
    clnt_sock->sock_len = sizeof(clnt_sock->sock_addr);

    clnt_sock->sock_fd = accept(serv_sock->sock_fd, (struct sockaddr*)&clnt_sock->sock_addr, &clnt_sock->sock_len);
    if(clnt_sock->sock_fd < 0) {
        perror("accept() error");
        return 0;
    }

    return 1;
}