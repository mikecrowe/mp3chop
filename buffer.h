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

#ifndef BUFFER_H
#define BUFFER_H 1

#include "types.h"
#include "byte_string.h"

class InsufficientDataException
{
};

class DataSource
{
public:
    virtual int ReadInto(BYTE *buffer, int bytes_required) = 0;
};

class InputStreamBuffer
{
    DataSource *source;
    BYTE *input_buffer;
    int input_size;
    int input_min;
    int input_writep;
    int input_readp;
    int buffer_start_offset;
    
protected:
    void ShoveUp();
    int Space() const;
    
public:
    InputStreamBuffer(int size, int lookbehind);
    ~InputStreamBuffer();
    DataSource *SetSource(DataSource *new_source)
    {
	DataSource *old_source = source;
	source = new_source;
	buffer_start_offset = 0;
	return old_source;
    }
    
    void EnsureAvailable(int count);
    const BYTE *GetPointer() const
    {
	return input_buffer + input_readp;
    }
    int GetOffset() const;
    int GetAvailable() const;
    void Advance(int count);
    void Rewind(int count);
    
    bool IsEOFAt(int count);
};

class DataSink
{
public:
    virtual int WriteOut(const BYTE *buffer, int bytes_available) = 0;
};

class OutputStreamBuffer
{
    DataSink *sink;
    byte_string output_buffer;
    bool bookmark_active;
    bool append_mode;
    int offset;
    
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
    
    void Append(const BYTE *start, int length);

    void Flush();

    int GetOffset() const
    {
	return offset;
    }
};


#endif
