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

    std::string json_string = "";
    bool f = 0;

    for (; request[i] != '\0'; i++) {
        if(request[i] == '}') {
            f = 0;
        }

        if (f) {
            json_string.push_back(request[i]);
        }

        if (request[i] == '{') {
            f = 1;
        }
    }

    parseJsonString(json_string);
}

void HttpParser::parseJsonString(std::string json_string) {
    if(json_string.size() == 0) {
        return;
    }
    json_string.push_back(',');

    std::string key, value;
    bool f = 0, isNumeric = 1, record = 0;

    for(int i = 0; i < json_string.size(); i++) {
        if(json_string[i] == '\n') {
            continue;
        }
        if (json_string[i] == ':') {
            f = 1;
            record = 0;
            continue;
        } else if(json_string[i] == ',') {
            if(isNumeric) {
                Integer* integer = new Integer();
                integer->value = std::stoi(value);
                data[key] = integer;
            } else {
                String* string = new String();
                string->value = value;
                data[key] = string;
            }
            key = "";
            value = "";
            f = 0;
            isNumeric = 1;
            record = 0;
            continue;
        }

        if (f == 0) {
            if(json_string[i] == '\"') {
                if(!record) {
                    key.clear();
                }
                record = 1;
                continue;
            }   
            key.push_back(json_string[i]);
        } else if (f == 1) {
            if(json_string[i] == '\"') {
                isNumeric = 0;
                if(!record) {
                    value.clear();
                }
                record = 1;
                continue;
            }

            value.push_back(json_string[i]);
        }
    }
}

int HttpParser::getInteger(std::string key) {
    try {
        return dynamic_cast<Integer*>(data[key]) -> value;
    } catch (std::exception& e) {
        std::cout << "Key not found" << std::endl;
        return -1;
    }
}

std::string HttpParser::getString(std::string key) {
    try {
        return dynamic_cast<String*>(data[key]) -> value;
    } catch (std::exception& e) {
        std::cout << "Key not found" << std::endl;
        return "-1";
    }
}

HttpParser::~HttpParser() {
    free(path);
}