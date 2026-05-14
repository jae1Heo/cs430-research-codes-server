#include "../../includes/server.h"


// port should be unsigned short and have to be converted into network byte order
int init_server(struct sock* serv_sock, const unsigned short un_port) {
    /*
    initializing as TCP socket
    may have to change this to UDP
    */
    serv_sock->sock_fd = socket(AF_INET, SOCK_STREAM, 0); 
    if(serv_sock->sock_fd < 0) {
        perror("socket() error");
        return 0;
    }

    memset((void*)&serv_sock->sock_addr, 0, sizeof(serv_sock->sock_addr)); // initialize socket address struct

    serv_sock->sock_addr.sin_family = AF_INET; // TCP ipv4
    serv_sock->sock_addr.sin_addr.s_addr = htonl(INADDR_ANY); // open for any client
    serv_sock->sock_addr.sin_port = un_port; // open for un_port port number (currently 12345)
    serv_sock->sock_len = sizeof(serv_sock->sock_addr); // size of address struct

    // bind the server socket
    if(bind(serv_sock->sock_fd, (struct sockaddr*)&serv_sock->sock_addr, serv_sock->sock_len) < 0) {
        perror("bind() error");
        return 0;
    }

    // make the server socket to listen for clients, queue length = 3
    if(listen(serv_sock->sock_fd, 3) < 0) {
        perror("listen() error");
        return 0;
    }

    return 1;
}


// called by client thread
int connect_client(struct sock* serv_sock, struct sock* clnt_sock) {
    if(serv_sock == NULL) { // errror checking
        fputs("server socket not initialized", stderr);
        return 0;
    }

    // initialize client socket address struct
    memset((void*)&clnt_sock->sock_addr, 0, sizeof(clnt_sock->sock_addr));
    clnt_sock->sock_len = sizeof(clnt_sock->sock_addr);

    // make the server socket to accept for incoming client connection
    clnt_sock->sock_fd = accept(serv_sock->sock_fd, (struct sockaddr*)&clnt_sock->sock_addr, &clnt_sock->sock_len);
    if(clnt_sock->sock_fd < 0) {
        perror("accept() error");
        return 0;
    }

    printf("client connected\n");

    return 1;
}

int send_all(struct sock* clnt_sock, const void* buffer, size_t buffer_len) {
    size_t total_sent = 0; // variable that counts total bytes sent to clients
    const unsigned char* partial = (const unsigned char*)buffer; // partial buffer
    
    while(total_sent < buffer_len) { // sending logic
        // sending data to client 
        ssize_t sent = send(clnt_sock->sock_fd, (const unsigned char*)partial + total_sent, buffer_len - total_sent, 0);
        if(sent <= 0) {
            return 0;
        }
        total_sent += sent;
    }

    return 1;
}

int recv_all(struct sock* clnt_sock, void* buffer, size_t buffer_len) {
    size_t total_recv = 0; // variable that counts total bytes received from clients
    const unsigned char* partial = (const unsigned char*)buffer; // partial buffer

    while(total_recv < buffer_len) {
        // receiving data from client
        ssize_t recvd = recv(clnt_sock->sock_fd, (const unsigned char*)partial + total_recv, buffer_len - total_recv, 0);
        if(recvd <= 0) {
            return 0;
        }
        total_recv += recvd;
    }
    return 1;
}

// warpper function of send_all() and recv_all()
int send_packet(struct sock* clnt_sock, const void* packet, uint16_t packet_len) {
    uint16_t net_len = htons(packet_len); // buffer holds processed packet (network byte order)
    // first let the client know the size of the incoming packet
    if(!send_all(clnt_sock, &net_len, sizeof(net_len))) {
        return 0;
    }
    
    // then send the actual packet to the client
    if(!send_all(clnt_sock, packet, packet_len)) {
        return 0;
    }

    return 1;
}

int recv_packet(struct sock* clnt_sock, void* buffer, uint16_t* packet_len) {
    uint16_t net_len; // buffer holds data from client
    // first receive the size of the incoming packet from the client
    if(!recv_all(clnt_sock, &net_len, sizeof(net_len))) {
        return 0;
    }

    uint16_t data_len = ntohs(net_len); // since the packet will be the network byte order, have to convert it to host 
    // then receive the actual packet from the client
    if(!recv_all(clnt_sock, buffer, data_len)) {
        return 0;
    }

    // save the size of the actual packet
    if (packet_len) { 
        *packet_len = data_len;
    }

    fputs("packet received\n", stdout);

    return 1;
}

void close_sock(struct sock* sk) {
    close(sk->sock_fd);
}