#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define CONNECTION_PORT 8080

void init_network_communication() {
    int connection_fd;
    struct sockaddr_in server_config;

    // Configure server address
    memset(&server_config, 0, sizeof(server_config));
    server_config.sin_family = AF_INET;
    server_config.sin_port = htons(CONNECTION_PORT);
    server_config.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Create socket
    if ((connection_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket initialization error");
        exit(EXIT_FAILURE);
    }

    // Establish connection
    if (connect(connection_fd, (struct sockaddr*)&server_config, sizeof(server_config)) == -1) {
        perror("Server connection error");
        close(connection_fd);
        exit(EXIT_FAILURE);
    }

    // Prepare and send data
    const char* student_info = "Kuznetsov Kirill Evgenievich KKSO-26-24 1st Course";
    if (send(connection_fd, student_info, strlen(student_info), 0) == -1) {
        perror("Data transmission error");
    }

    // Close connection
    close(connection_fd);
}

int main() {
    init_network_communication();
    return 0;
}

