/*
 * mp3chop
 *
 * (C) 2000-2020 Mike Crowe <mac@mcrowe.com>
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

#include "file_data_sink.h"
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif
#ifdef HAVE_IO_H
#include <io.h>
#endif

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

int FileDataSink::WriteOut(const uint8_t *buffer, size_t bytes_available)
{
    size_t total_bytes = 0;
    
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


