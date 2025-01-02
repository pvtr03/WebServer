#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "socket.h" 
#include "httpParser.h"
#include "httpResponse.h"

const int BUFFER_SIZE = 1024;

void handle_client(int client_socket, Socket& server) {
    std::cout << "Handling client connection...\n";

    char request[BUFFER_SIZE];
    int byte_size = server.recieveData(client_socket, request, BUFFER_SIZE);
    request[byte_size] = '\0';
    
    HttpParser parser(request);

    if(strcmp(parser.path, "/") == 0 && parser.method == HttpMethod::GET) {
        std::ifstream file("../pages/index.html");
        if (file.is_open()) {
            std::stringstream ss;
            ss << file.rdbuf();
            std::string body = ss.str();
            HttpResponse response(200, "OK", body);
            server.sendData(client_socket, response.response);
            return;
        }
    }
    
    std::string body = "<html><body><h1>Not Found</h1></body></html>";
    std::string response = 
        "HTTP/1.1 404 NOT FOUND\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " + std::to_string(body.length()) + "\r\n"
        "\r\n" + body;

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