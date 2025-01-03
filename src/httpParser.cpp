#include "httpParser.h"

#include <string>
#include <iostream>
#include <cstring> 

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
        if (f) {
            json_string.push_back(request[i]);
        }

        if (request[i] == '{') {
            f = 1;
        }
    }

    if(json_string.size() == 0) {
        return;
    }

    while(json_string[json_string.size() - 1] != '}') {
        json_string.pop_back();
    }
    json_string.pop_back();

    parseJsonString(json_string);
}


void HttpParser::parseJsonString(std::string json_string) {
    if(json_string.size() == 0) 
        return;
    
    std::string key, value;
    bool capture_value = 0, recording = 0, cnt = 0;

    for(int i = 0; i < json_string.size(); i++) {
        // std::cout << key << " " << value << std::endl;        
        if (recording) {
            std::string current;
            if(json_string[i] == '\\') {
                if(json_string[i + 1] == '\\' && json_string[i + 2] == 'n') {
                    current.push_back('\\');
                    current.push_back('n');
                    i += 2;
                } else if(json_string[i + 1] == 'n') {
                    current.push_back('\n');
                    i++;
                } else {
                    i++;
                    current.push_back(json_string[i]);
                }
            } else if(json_string[i] == '\"') {
                // std::cout << "here" << key << " : " << value << " " << std::endl;
                if (capture_value) {
                    String* string = new String();
                    string->value = value;
                    data[key] = string;
                    key = "";
                    value = "";
                }

                recording = 0;
                capture_value = 0;
            } else {
                current.push_back(json_string[i]);
            } 

            if (capture_value) {
                value += current;
            } else {
                key += current;
            }

            continue;
        }

        if(json_string[i] == '\"') {
            cnt ^= 1;
            recording = 1;
            continue;
        }

        if (json_string[i] == ':') {
            capture_value = 1;
            continue;
        }

    }
}

/*
void HttpParser::parseJsonString(std::string json_string) {
    std::cout << json_string << std::endl;
    if(json_string.size() == 0) {
        return;
    }
    json_string.push_back(',');

    std::string key, value;
    bool f = 0, isNumeric = 1, record = 0;

    for(int i = 0; i < json_string.size(); i++) {
        // std::cout << value << std::endl;
        if (record == 0 && json_string[i] == ':') {
            f = 1;
            record = 0;
            continue;
        } else if(json_string[i] == ',' && f == 0 && record == 0) {
            if(isNumeric) {
                std::cout << "! " << key << " " << value << std::endl;
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
                record ^= 1;
                continue;
            }   
            key.push_back(json_string[i]);
        } else if (f == 1) {
            if(json_string[i] == '\\') {
                if(json_string[i + 1] == '\\' && json_string[i + 2] == 'n') {
                    value.push_back('\\');
                    value.push_back('n');
                    i += 2;
                    continue;
                } else if(json_string[i + 1] == 'n') {
                    value.push_back('\n');
                    i++;
                    continue;
                }
                
                i++;
                value.push_back(json_string[i]);
                continue;
            } 

            if(json_string[i] == '\"') {
                isNumeric = 0;
                if(!record) {
                    value.clear();
                }
                record ^= 1;
                continue;
            }

            value.push_back(json_string[i]);
        }
    }
}*/

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
    delete[] path;
    for(auto it = data.begin(); it != data.end(); it++) {
        delete it->second;
    }
}