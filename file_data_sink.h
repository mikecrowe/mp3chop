#ifndef FILE_DATA_SINK_H
#define FILE_DATA_SINK_H 1

#include "file_exception.h"
#include "buffer.h"

class FileDataSink : public DataSink
{
    bool opened;
    int fd;
    
public:
    FileDataSink();
    virtual ~FileDataSink();
    void OpenStandardOutput();
    void Close();

    virtual int WriteOut(const BYTE *buffer, int bytes_available);
};

#endif
