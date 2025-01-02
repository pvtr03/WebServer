#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <string>

class HttpResponse {
public:
    int status;
    char* response;
    int size;

    HttpResponse(int status, std::string statusName, std::string content);
};

#endif