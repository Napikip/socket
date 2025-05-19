#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_PORT 8080
#define BUFFER_SIZE 256
#define MAX_PENDING 5

void setup_server() {
    int master_socket, client_connection;
    struct sockaddr_in server_addr;
    char recv_buffer[BUFFER_SIZE];
    
    // Initialize server configuration
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Create master socket
    if ((master_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    // Bind socket to port
    if (bind(master_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed");
        close(master_socket);
        exit(EXIT_FAILURE);
    }

    // Start listening
    if (listen(master_socket, MAX_PENDING) < 0) {
        perror("Listen failed");
        close(master_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server running on port %d...\n", SERVER_PORT);

    // Accept incoming connection
    if ((client_connection = accept(master_socket, NULL, NULL)) < 0) {
        perror("Accept failed");
        close(master_socket);
        exit(EXIT_FAILURE);
    }
    printf("New client connected!\n");

    // Receive and process data
    ssize_t bytes_read = recv(client_connection, recv_buffer, BUFFER_SIZE-1, 0);
    if (bytes_read > 0) {
        recv_buffer[bytes_read] = '\0';
        printf("Received data: ");
        for (int i = 0; i < bytes_read && i < 53; i++) {
            putchar(recv_buffer[i]);
        }
        putchar('\n');
    } else if (bytes_read < 0) {
        perror("Receive error");
    }

    // Cleanup
    close(client_connection);
    close(master_socket);
}

int main() {
    setup_server();
    return EXIT_SUCCESS;
}
