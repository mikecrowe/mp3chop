#include "file_data_sink.h"
#include <unistd.h>
#include <fcntl.h>

FileDataSink::FileDataSink()
    : opened(false), fd(-1)
{
}

FileDataSink::~FileDataSink()
{
    Close();
}

void FileDataSink::OpenStandardOutput()
{
    Close();
    fd = 1;
    opened = false;
}

void FileDataSink::Close()
{
    if (opened)
    {
	close(fd);
    }
    fd = -1;
}

int FileDataSink::WriteOut(const BYTE *buffer, int bytes_available)
{
    int total_bytes = 0;
    
    // We may take more than one write to send all the data.
    while (total_bytes < bytes_available)
    {
	int bytes = write(fd, buffer, bytes_available);
	if (bytes < 0)
	    throw FileException();
	total_bytes += bytes;
    }
    return total_bytes;
}


