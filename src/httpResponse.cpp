#include "httpResponse.h"

#include <sstream>
#include <string>

HttpResponse::HttpResponse(int status, std::string statusName,  std::string content) {
    size = 0;
    std::stringstream ss;

    ss << "HTTP/1.1 ";
    ss << std::to_string(status) << " ";
    ss << status << "\n";
    ss << "Content-Type: " << "text/html" << "\n";
    ss << "Content-Length: " << std::to_string(content.size()) << "\n\n";

    std::string header = ss.str();
    this->response = new char[header.size() + content.size() + 4];
    for (int i = 0; i < header.size(); i++)
        response[size++] = header[i];
    for (int i = 0; i < content.size(); i++)
        response[size++] = content[i];
}