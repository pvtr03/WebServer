#include "httpParser.h"

#include <string>
#include <iostream>

HttpParser::HttpParser(char* request) {
    
    int i = 0;

    std::string req = "";
    for (; request[i] != ' '; i++) {
        req.push_back(request[i]);
    }

    if (req == "GET") {
        method = HttpMethod::GET;
    } else if(req == "POST") {
        method = HttpMethod::POST;
    } else if(req == "PUT") {
        method = HttpMethod::PUT;
    } else if(req == "DELETE") {
        method = HttpMethod::DELETE;
    } else {
        std::cout << "Invalid HTTP Method" << std::endl;
        exit(1);
    }

    i++;
    std::string path = "";
    for (; request[i] != ' '; i++) {
        path.push_back(request[i]);
    }
    this->path = new char[path.size() + 1];
    strcpy(this->path, path.c_str());

    std::cout << "Method: " << req << std::endl;
    std::cout << "Path: " << path << std::endl;
}