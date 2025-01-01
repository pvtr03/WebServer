#include <iostream>
#include <string>

#include "socket.h" 
#include "httpParser.h"

const int BUFFER_SIZE = 1024;

void handle_client(int client_socket, Socket& server) {
    std::cout << "Handling client connection...\n";

    char request[BUFFER_SIZE];
    int byte_size = server.recieveData(client_socket, request, BUFFER_SIZE);
    request[byte_size] = '\0';
    
    HttpParser parser(request);
    
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