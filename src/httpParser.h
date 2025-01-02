#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include "json.h"
#include <map>

enum class HttpMethod {
    GET,
    POST,
    PUT,
    DELETE
};

class HttpParser {
    std::map<std::string, JSON*> data;
public:
    HttpMethod method;
    char* path;
    
    HttpParser(char* request);
    void parseJsonString(std::string json_string);
    int getInteger(std::string key);
    std::string getString(std::string key);
    ~HttpParser();
};

#endif