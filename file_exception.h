#ifndef FILE_EXCEPTION_H
#define FILE_EXCEPTION_H 1

#include <string>
#include <errno.h>

#if !defined(HAVE_STRERROR)
#include <stringstream>
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
	std::ostringstream ss;
	ss << "error " << error_number;
	return ss.str();
    }
#endif
    int Number() { return error_number; }
};

#endif
