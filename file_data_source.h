#ifndef FILE_DATA_SOURCE_H
#define FILE_DATA_SOURCE_H 1

#include <string>
#include <errno.h>
#include "buffer.h"

class FileException
{
    int error_number;
    
public:
    FileException() : error_number(errno) {}
    FileException(int e) : error_number(e) {}
    
    const std::string Description() { return strerror(error_number); }
    int Number() { return error_number; }
};

class FileDataSource : public DataSource
{
    bool opened;
    int fd;
    
public:
    FileDataSource();
    virtual ~FileDataSource();
    void Open(const std::string &filename);
    void OpenStandardInput();
    void Close();
    
    virtual int ReadInto(BYTE *buffer, int bytes_required);
};

#endif
