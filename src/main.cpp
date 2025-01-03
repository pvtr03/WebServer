#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>

#include "socket.h" 
#include "httpParser.h"
#include "httpResponse.h"
#include "config.h"

const int BUFFER_SIZE = 10240;

void handle_client(int client_socket, Socket& server);
bool serveLandingPage(int client_socket, HttpParser& parser, Socket& server);
bool compileCode(int client_socket, HttpParser& parser, Socket& server);
std::string exec(const char* cmd);
std::string jsonify(std::string s);

int main() {
    std::cout << "Hello, World!" << std::endl;
    Socket server(PORT); 

    while(true) {
        int client = server.acceptConnection();
        handle_client(client, server);
    }

    return 0;
}

void handle_client(int client_socket, Socket& server) {
    std::cout << "Handling client connection...\n";

    char request[BUFFER_SIZE];
    int byte_size = server.recieveData(client_socket, request, BUFFER_SIZE);
    request[byte_size] = '\0';
    
    HttpParser parser(request);

    if (strcmp(parser.path, "/") == 0 && parser.method == HttpMethod::GET) {
        if (serveLandingPage(client_socket, parser, server)) {
            return;
        }   
    }

    if (strcmp(parser.path, "/") == 0 && parser.method == HttpMethod::POST) {
        if (compileCode(client_socket, parser, server)) {
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

bool serveLandingPage(int client_socket, HttpParser& parser, Socket& server) {
    std::ifstream file("../pages/index.html");
    if (file.is_open()) {
        std::stringstream ss;
        ss << file.rdbuf();
        std::string body = ss.str();
        HttpResponse response(200, "OK", body);
        server.sendData(client_socket, response.response);
        return 1;
    } 

    return 0;
}

bool compileCode(int client_socket, HttpParser& parser, Socket& server) {
    std::string code = parser.getString("code");
    std::string input = parser.getString("input");

    std::string filename = "code.cpp";
    std::string executable = "code_executable";
    std::string inputFile = "input.txt";
    std::string outputFile = "output.txt";

    std::ofstream codeFile(filename);
    codeFile << code;
    codeFile.close();

    std::ofstream inputFileStream(inputFile);
    inputFileStream << input;
    inputFileStream.close();

    std::string compileCommand = "g++ " + filename + " -o " + executable + " 2>&1";
    std::string compileOutput = exec(compileCommand.c_str());

    if (!compileOutput.empty()) {
        std::string body = "{ \"output\": \"" + jsonify(compileOutput) + "\" }";
        HttpResponse response(200, "OK", body);
        server.sendData(client_socket, response.response);
        return 1;
    }

    std::string runCommand = "./" + executable + " < " + inputFile + " > " + outputFile + " 2>&1";
    std::string runOutput = exec(runCommand.c_str());

    std::ifstream outputFileStream(outputFile);
    std::stringstream outputBuffer;
    outputBuffer << outputFileStream.rdbuf();
    std::string output = outputBuffer.str();
    outputFileStream.close();

    std::string body = "{ \"output\": \"" + jsonify(output) + "\" }";
    HttpResponse response(200, "OK", body);
    server.sendData(client_socket, response.response);
    return 1;
}

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::string jsonify(std::string s) {
    std::string json = "";
    for (char c : s) {
        if (c == '\n') {
            json += "\\n";
        } else if (c == '\"') {
            json += "\"";
        } else {
            json += c;
        }
    }
    return json;
}