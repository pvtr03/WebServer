#ifndef JSON_H
#define JSON_H

#include <vector>
#include <iostream>

class JSON {
public:
    virtual ~JSON() {}
    virtual void print() = 0;
};

class Integer : public JSON {
public:
    int value;
    void print() {
        std::cout << value << std::endl;
    }
};

class String : public JSON {
public:
    std::string value;
    void print() {
        std::cout << value << std::endl;
    }
};

class Array : public JSON {
public:
    std::vector<JSON*> value;
};

#endif