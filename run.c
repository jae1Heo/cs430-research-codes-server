#include "server/server.h"

int main(int argc, char* argv[]) {

    /*
        testing code use with testclient
    */
    struct sock serv_sock;
    struct sock clnt_sock;

    memset((void*)&serv_sock, 0, sizeof(struct sock));
    memset((void*)&clnt_sock, 0, sizeof(struct sock));


    if(!init_server(&serv_sock, htons(9111))) {
        return 1;
    }

    if(!connect_client(&serv_sock, &clnt_sock)) {
        return 1;
    }

    write(clnt_sock.sock_fd, "is it worked?", 13);
    char buffer[256];
    int bread = read(clnt_sock.sock_fd, buffer, 256);
    printf("%s\n", buffer);

    close(clnt_sock.sock_fd);
    close(serv_sock.sock_fd);

    return 0;
}