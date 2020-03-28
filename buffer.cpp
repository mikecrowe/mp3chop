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

#include <string.h>
#include "buffer.h"
#include <stdio.h>
#include <ctype.h>
#include "assert.h"

InputStreamBuffer::InputStreamBuffer(size_t size, size_t lookbehind)
    : source(0), input_size(size), input_min(lookbehind),
      input_writep(0), input_readp(0), buffer_start_offset(0)
{
    input_buffer = reinterpret_cast<uint8_t *>(operator new(size));
}

InputStreamBuffer::~InputStreamBuffer()
{
    delete input_buffer;
}

void InputStreamBuffer::ShoveUp()
{
    // if it's worth doing push everything up to the start of the
    // buffer + minimum input value leaving space at the end for new
    // data.
    if (input_readp > input_min)
    {
	if (input_readp > input_min)
	{
	    size_t begin = input_readp - input_min;
	    if (begin < input_size)
	    {
		int bytes_kept = input_writep - begin;
		if (bytes_kept)
		    memmove(input_buffer, input_buffer + begin, bytes_kept);
	    }
	    else
		begin = input_size;
	    
	    input_readp -= begin;
	    input_writep -= begin;
	    buffer_start_offset += begin;
	}
    }
}

uint64_t InputStreamBuffer::GetOffset() const
{
    return buffer_start_offset + input_readp;
}

size_t InputStreamBuffer::Space() const
{
    assert(input_size > input_writep);
    return input_size - input_writep;
}

size_t InputStreamBuffer::GetAvailable() const
{
    if (input_writep < input_readp)
	return 0;
    return input_writep - input_readp;
}

void InputStreamBuffer::EnsureAvailable(size_t count)
{
    // We can't be asked for more than the input size.
    assert(count <= input_size);
    
    if (GetAvailable() < count)
    {
	do
	{
	    ShoveUp();
	    input_writep += source->ReadInto(input_buffer + input_writep, Space());
	}
	while (input_readp > input_size);
	
	if (GetAvailable() < count)
	    throw InsufficientDataException();
    }
}

void InputStreamBuffer::Advance(uint64_t count)
{
    input_readp += count;
}						

void InputStreamBuffer::Rewind(size_t count)
{
    if (input_readp >= count)
	input_readp -= count;
    else
	throw InsufficientDataException();
}

// Keep reading data until either we have _more_ than count or we find
// EOF.
bool InputStreamBuffer::IsEOFAt(size_t count)
{
    while (GetAvailable() < count)
    {
	uint8_t b = input_buffer[input_readp];
	fprintf(stderr, "In IsEOFAt(%zu) with %zu currently available - first is '%c' (%d)\n",
		count, GetAvailable(), isprint(b) ? b : '.', b);
	
	ShoveUp();
	
	if (Space() < count)
	    return false;
	
	int bytes_read = source->ReadInto(input_buffer + input_writep, Space());
	if (bytes_read)
	{
	    input_writep += bytes_read;
	}
	else
	{
	    fprintf(stderr, "Got zero byte read with %zu available\n", GetAvailable());
	    // We got a zero byte read - that means EOF!
	    return (GetAvailable() == count);
	}
    }
    return false;
}

OutputStreamBuffer::OutputStreamBuffer()
    : sink(NULL),
      bookmark_active(false),
      append_mode(true),
      offset(0)
{
}

OutputStreamBuffer::~OutputStreamBuffer()
{
}

void OutputStreamBuffer::Flush()
{
    if (!bookmark_active)
    {
	size_t n = sink->WriteOut(output_buffer.data(), output_buffer.length());
	output_buffer.erase(0, n);
    }
}

void OutputStreamBuffer::Append(const uint8_t *begin, size_t length)
{
    if (append_mode)
    {
	output_buffer.append(begin, length);
	if (output_buffer.size() > 8192)
	    Flush();

	offset += length;
    }
    else
    {
	// If we're not in append mode then just write it out
	// directly and then throw enough away from the string to
	// cover it.
	const uint8_t *end = begin + length;
	while (begin < end)
	{
	    size_t n = sink->WriteOut(begin, length);
	    output_buffer.erase(0, n);
	    begin += n;
	}
    }
}

void OutputStreamBuffer::SetBookmark()
{
    Flush();
    bookmark_active = true;
}

void OutputStreamBuffer::ClearBookmark()
{
    bookmark_active = false;
    append_mode = false;
    Flush();
}

void OutputStreamBuffer::GoToBookmark()
{
    append_mode = false;
}
