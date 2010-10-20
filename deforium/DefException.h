#ifndef __DEFEXCEPTION_H__
#define __DEFEXCEPTION_H__

#include <stdexcept>

class DefException : public std::runtime_error{
public:
    DefException(const std::string& msg) 
        : std::runtime_error(msg) {};
};

#endif  // __DEFEXCEPTION_H__
