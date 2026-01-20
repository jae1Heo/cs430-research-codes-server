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

    int yes = 1;
    if(!setsockopt(serv_sock->sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes))) {
        perror("setsockopt() error");
        return 0;
    }

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

int send_all(struct sock* clnt_sock, const void* buffer, size_t buffer_len) {
    size_t total_sent = 0;
    const unsigned char* partial = (const unsigned char*)buffer;
    
    while(total_sent < buffer_len) {
        ssize_t sent = send(clnt_sock->sock_fd, partial + total_sent, buffer_len - total_sent, 0);
        if(sent <= 0) {
            return 0;
        }
        total_sent += sent;
    }

    return 1;
}

int recv_all(struct sock* clnt_sock, void* buffer, size_t buffer_len) {
    size_t total_recv = 0;
    const unsigned char* partial = (const unsigned char*)buffer;

    while(total_recv < buffer_len) {
        ssize_t recvd = recv(clnt_sock->sock_fd, partial + total_recv, buffer_len - total_recv, 0);
        if(recvd <= 0) {
            return 0;
        }
        total_recv += recvd;
    }
    return 1;
}

int send_packet(struct sock* clnt_sock, const void* packet, uint16_t packet_len) {
    uint16_t net_len = htons(packet_len);
    if(!send_all(clnt_sock, &net_len, sizeof(net_len))) {
        return 0;
    }

    if(!send_all(clnt_sock, packet, packet_len)) {
        return 0;
    }

    return 1;
}

int recv_packet(struct sock* clnt_sock, void* buffer, uint16_t* packet_len) {
    uint16_t net_len;
    if(!recv_all(clnt_sock, &net_len, sizeof(net_len))) {
        return 0;
    }

    uint16_t data_len = ntohs(net_len);
    if(!recv_all(clnt_sock, buffer, data_len)) {
        return 0;
    }

    if (packet_len) {
        *packet_len = data_len;
    }

    return 1;
}