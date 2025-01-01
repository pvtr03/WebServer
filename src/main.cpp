#include <iostream>
#include <string>
#include "socket.h" // Assuming you have a header file for the Socket class

void handle_client(int client_socket, Socket& server) {
    std::cout << "Handling client connection...\n";

    char buffer[1024];
    server.recieveData(client_socket, buffer);
    // if (bytes_received > 0) {
    //     buffer[bytes_received] = '\0'; // Null-terminate the buffer
    // }

    // std::cout << buffer << std::endl;
    std::string response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 48\r\n"
        "\r\n"
        "<html><body><h1>Hello, World!</h1></body></html>";
    const char* data = response.c_str();
    server.sendData(client_socket, data);
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    Socket server(8080);

    while(true) {
        int client = server.acceptConnection();
        handle_client(client, server);
    }

    return 0;
}