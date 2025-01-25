#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MY_PORT "2200"
#define BACKLOG 10
#define BUFFER_SIZE 256

int main(void) {
    int yes = 1;
    struct sockaddr_storage their_addr;
    struct addrinfo hints, *res;
    int sock_fd, new_fd;
    char ipstr[INET6_ADDRSTRLEN];
    char buff[BUFFER_SIZE];
    socklen_t addr_size;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, MY_PORT, &hints, &res) != 0) {
        fprintf(stderr, "Failed to get address information\n");
        return 2;
    }

    sock_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock_fd == -1) {
        perror("Socket creation failed");
        freeaddrinfo(res);
        return 2;
    }

    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        perror("Setsockopt failed");
        close(sock_fd);
        freeaddrinfo(res);
        return 2;
    }

    if (bind(sock_fd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("Bind failed");
        close(sock_fd);
        freeaddrinfo(res);
        return 2;
    }

    freeaddrinfo(res);

    if (listen(sock_fd, BACKLOG) == -1) {
        perror("Listen failed");
        close(sock_fd);
        return 2;
    }

    addr_size = sizeof their_addr;
    new_fd = accept(sock_fd, (struct sockaddr *)&their_addr, &addr_size);
    if (new_fd == -1) {
        perror("Accept failed");
        close(sock_fd);
        return 2;
    }

    int bytes_received = recv(new_fd, buff, BUFFER_SIZE - 1, 0);
    if (bytes_received == -1) {
        perror("Receive failed");
        close(new_fd);
        close(sock_fd);
        return 2;
    }
    buff[bytes_received] = '\0';

    printf("Received message: %s\n", buff);

    int bytes_sent = send(new_fd, buff, bytes_received, 0);
    if (bytes_sent == -1) {
        perror("Send failed");
    } else {
        printf("Echoed message back to client\n");
    }

    close(new_fd);
    close(sock_fd);
    return 0;
}

