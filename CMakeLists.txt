cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(WebServer VERSION 1.0)

set(SOURCES     src/main.cpp
                src/socket.cpp
                src/httpParser.cpp
                src/httpResponse.cpp
)

add_executable(WebServer ${SOURCES})