/*
 * mp3chop
 *
 * (C) 2000-2010 Mike Crowe <mac@mcrowe.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */

#include "file_data_source.h"
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif
#ifdef HAVE_IO_H
#include <io.h>
#endif


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
    }
    fd = -1;
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

