#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define SERVER_PORT "2200"
#define BUFFER_SIZE 256

int main() {
    struct addrinfo hints, *server_res;
    int sock_fd;
    char buff[BUFFER_SIZE];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo("127.0.0.1", SERVER_PORT, &hints, &server_res) != 0) {
        fprintf(stderr, "Failed to get address information\n");
        return 2;
    }

    sock_fd = socket(server_res->ai_family, server_res->ai_socktype, server_res->ai_protocol);
    if (sock_fd == -1) {
        perror("Socket creation failed");
        freeaddrinfo(server_res);
        return 2;
    }

    if (connect(sock_fd, server_res->ai_addr, server_res->ai_addrlen) == -1) {
        perror("Failed to connect to server");
        close(sock_fd);
        freeaddrinfo(server_res);
        return 2;
    }

    freeaddrinfo(server_res);

    char *message = "Hello, server!";
    if (send(sock_fd, message, strlen(message), 0) == -1) {
        perror("Failed to send message");
        close(sock_fd);
        return 2;
    }

    printf("Message sent to server: %s\n", message);

    int bytes_received = recv(sock_fd, buff, BUFFER_SIZE - 1, 0);
    if (bytes_received == -1) {
        perror("Failed to receive message");
        close(sock_fd);
        return 2;
    }
    buff[bytes_received] = '\0';

    printf("Message received from server: %s\n", buff);

    close(sock_fd);
    return 0;
}

