#ifndef FILE_DATA_SOURCE_H
#define FILE_DATA_SOURCE_H 1

#include "file_exception.h"
#include "buffer.h"

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
