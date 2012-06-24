#define _POSIX_C_SOURCE 200112L

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/select.h>

int main(int argc, char **argv)
{
    int err, socket_fd;
    struct addrinfo hints;
    struct addrinfo *srv;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    // Connect
    if ((err = getaddrinfo("localhost", "4242", &hints, &srv)) != 0)
        return -1;
    if ((socket_fd = socket(srv->ai_family, srv->ai_socktype, 0)) < 0)
        return -1;
    if ((err = connect(socket_fd, srv->ai_addr, srv->ai_addrlen)) != 0)
        return -1;
    freeaddrinfo(srv);

    // Select param
    fd_set socket_set;
    FD_ZERO(&socket_set);
    FD_SET(STDIN_FILENO, &socket_set);
    FD_SET(socket_fd, &socket_set);

    // Set rand seed
    srand(time(NULL));
    do {
        printf("Sending malformed packet :D\n");
        uint32_t packet_size = rand() % (1 << 22);
        if (packet_size == 0) packet_size = 1;
        packet_size = htonl(packet_size);
        send(socket_fd, &packet_size, 4, 0);

        unsigned char qmetatype = rand() % 255;
        send(socket_fd, &qmetatype, 1, 0);
        for (int i=0; i<packet_size; i++) {
            int to_send = rand();
            if (send(socket_fd, &to_send, sizeof(int), 0) < 1)
                return -1;
        }
    } while (read(socket_fd, &err, sizeof(int))); 
    close(socket_fd);
}
