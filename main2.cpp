#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

const int PORT_NUMBER = 0;
const int BUFFER_SIZE = 1024;

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;
    // Create socket
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        error("Error: Unable to create socket");
    }

    // Prepare server address
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT_NUMBER);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind socket to address
    if (bind(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        std::cout << "Error: Unable to bind socket to address" << std::endl;
    }

    // Listen for incoming connections
    if (listen(sock_fd, 5) == -1) {
        error("Error: Unable to listen on socket");
    }

    // Accept incoming connection
    int client_fd = accept(sock_fd, NULL, NULL);
    if (client_fd == -1) {
        error("Error: Unable to accept connection");
    }

    // Chat loop
    char buffer[BUFFER_SIZE];
    while (true) {
        // Receive message from client
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
        if (bytes_received == -1) {
            error("Error: Unable to receive data");
        }

        // Print message
        std::cout << "Client: " << buffer << std::endl;

        // Get message from user
        std::cout << "You: ";
        std::cin.getline(buffer, sizeof(buffer));

        // Send message to client
        int bytes_sent = send(client_fd, buffer, strlen(buffer), 0);
        if (bytes_sent == -1) {
            error("Error: Unable to send data");
        }

        // Check if user wants to quit
        if (strcmp(buffer, "quit") == 0) {
            break;
        }
    }

    // Close socket
    close(client_fd);
    close(sock_fd);

    return 0;
}
