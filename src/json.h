#ifndef JSON_H
#define JSON_H
#endif

#include <string>
#include <map>

class JSON {
public:
    std::map<std::string, JSON*> data;
};

class Integer : public JSON {
public:
    int value;
};

class String : public JSON {
public:
    std::string value;
};

class Array : public JSON {
public:
    std::vector<JSON*> data;
};


