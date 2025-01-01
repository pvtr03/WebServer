#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H
#endif

#include "json.h"

enum class HttpMethod {
    GET,
    POST,
    PUT,
    DELETE
};

class HttpParser {
    HttpMethod method;
    char* path;
    JSON data;
public:
    HttpParser(char* request);
};