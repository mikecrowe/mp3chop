#ifndef FILE_EXCEPTION_H
#define FILE_EXCEPTION_H 1

#include <string>
#include <errno.h>

class FileException
{
    int error_number;
    
public:
    FileException() : error_number(errno) {}
    FileException(int e) : error_number(e) {}
    
    const std::string Description() { return strerror(error_number); }
    int Number() { return error_number; }
};

#endif
