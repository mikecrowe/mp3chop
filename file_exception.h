#ifndef FILE_EXCEPTION_H
#define FILE_EXCEPTION_H 1

#include <string>
#include <errno.h>
#include "platform_config.h"

#if !defined(HAVE_STRERROR)
#include <cstdio>
#endif

class FileException
{
    int error_number;
    
public:
    FileException() : error_number(errno) {}
    FileException(int e) : error_number(e) {}

#if defined(HAVE_STRERROR)
    const std::string Description() { return strerror(error_number); }
#else
    const std::string Description()
    {
	char buffer[40];
	std::sprintf(buffer, "error %d", error_number);
	return std::string(buffer);
    }
#endif
    int Number() { return error_number; }
};

#endif
