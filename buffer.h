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

#ifndef BUFFER_H
#define BUFFER_H 1

#include "types.h"
#include "byte_string.h"

class InsufficientDataException
{
};

class DataSource
{
protected:
    virtual ~DataSource() {}
public:
    virtual size_t ReadInto(uint8_t *buffer, size_t bytes_required) = 0;
};

class InputStreamBuffer
{
    DataSource *source;
    uint8_t *input_buffer;
    size_t input_size;
    size_t input_min;
    size_t input_writep;
    uint64_t input_readp; //< Needs to be 64-bit so we can advance by 4GiB or more.
    uint64_t buffer_start_offset;
    
protected:
    void ShoveUp();
    size_t Space() const;
    
public:
    InputStreamBuffer(size_t size, size_t lookbehind);
    ~InputStreamBuffer();
    DataSource *SetSource(DataSource *new_source)
    {
	DataSource *old_source = source;
	source = new_source;
	buffer_start_offset = 0;
	return old_source;
    }
    
    void EnsureAvailable(size_t count);
    const uint8_t *GetPointer() const
    {
	return input_buffer + input_readp;
    }
    uint64_t GetOffset() const;
    size_t GetAvailable() const;
    void Advance(uint64_t count);
    void Rewind(size_t count);
    
    bool IsEOFAt(size_t count);
};

class DataSink
{
protected:
    virtual ~DataSink() {}
public:
    virtual int WriteOut(const uint8_t *buffer, size_t bytes_available) = 0;
};

class OutputStreamBuffer
{
    DataSink *sink;
    byte_string output_buffer;
    bool bookmark_active;
    bool append_mode;
    uint64_t offset;
    
protected:
    void Output();
    
public:
    OutputStreamBuffer();
    ~OutputStreamBuffer();
    DataSink *SetSink(DataSink *new_sink)
    {
	DataSink *old_sink = sink;
	sink = new_sink;
	return old_sink;
    }

    void SetBookmark();
    void ClearBookmark();
    void GoToBookmark();
    
    void Append(const uint8_t *start, size_t length);

    void Flush();

    uint64_t GetOffset() const
    {
	return offset;
    }
};


#endif
