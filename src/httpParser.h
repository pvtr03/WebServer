#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H
#endif

#include "json.h"
#include <map>

enum class HttpMethod {
    GET,
    POST,
    PUT,
    DELETE
};

class HttpParser {
    HttpMethod method;
    char* path;
    std::map<std::string, JSON*> data;
public:
    HttpParser(char* request);
    void parseJsonString(std::string json_string);
    int getInteger(std::string key);
    std::string getString(std::string key);
    ~HttpParser();
};