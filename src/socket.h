#ifndef SOCKET_H
#define SOCKET_H
#endif

#include <netinet/in.h>

class Socket {
private:
    int socketID;
    int port;
    sockaddr_in server_address;
public: 
    Socket(int port);
    int acceptConnection();
    int recieveData(int client, char* request, int size);
    void sendData(int client, const char* response);
    ~Socket(); 
};