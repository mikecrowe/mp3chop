#include "file_data_source.h"
#include <unistd.h>
#include <fcntl.h>

FileDataSource::FileDataSource()
    : opened(false), fd(-1)
{
}

FileDataSource::~FileDataSource()
{
    Close();
}

void FileDataSource::Open(const std::string &filename)
{
    Close();
    fd = open(filename.c_str(), O_RDONLY);
    if (fd < 0)
	throw FileException();
    opened = true;
}

void FileDataSource::OpenStandardInput()
{
    Close();
    fd = 0;
    opened = false;
}

void FileDataSource::Close()
{
    if (opened)
    {
	close(fd);
	fd = -1;
    }
}

int FileDataSource::ReadInto(BYTE *buffer, int bytes_required)
{
    int total_bytes = 0;
    
    // We may take more than one read to get all the data.
    while (total_bytes < bytes_required)
    {
	int bytes = read(fd, buffer, bytes_required);
	if (bytes < 0)
	    throw FileException();
	if (bytes == 0)
	    break;
	total_bytes += bytes;
    }
    return total_bytes;
}

