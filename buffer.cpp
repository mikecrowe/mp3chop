#include <string.h>
#include "buffer.h"
#include <stdio.h>
#include <ctype.h>

InputStreamBuffer::InputStreamBuffer(int size, int lookbehind)
    : source(0), input_size(size), input_min(lookbehind), input_writep(0), input_readp(0)
{
    input_buffer = reinterpret_cast<BYTE *>(operator new(size));
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
	int begin = input_readp - input_min;
	if (begin > 0)
	{
	    int bytes_kept = input_writep - begin;
	    if (bytes_kept)
		memmove(input_buffer, input_buffer + begin, bytes_kept);
	    input_readp -= begin;
	    input_writep -= begin;
	}
    }
}

int InputStreamBuffer::Space() const
{
    return input_size - input_writep;
}

int InputStreamBuffer::GetAvailable() const
{
    return input_writep - input_readp;
}

void InputStreamBuffer::EnsureAvailable(int count)
{
    if (GetAvailable() < count)
    {
	ShoveUp();
	
	input_writep += source->ReadInto(input_buffer + input_writep, Space());
	
	if (GetAvailable() < count)
	    throw InsufficientDataException();
    }
}

void InputStreamBuffer::Advance(int count)
{
    input_readp += count;
}						

void InputStreamBuffer::Rewind(int count)
{
    if (input_readp >= count)
	input_readp -= count;
    else
	throw InsufficientDataException();
}

// Keep reading data until either we have _more_ than count or we find
// EOF.
bool InputStreamBuffer::IsEOFAt(int count)
{
    while (GetAvailable() < count)
    {
	BYTE b = input_buffer[input_readp];
	fprintf(stderr, "In IsEOFAt(%d) with %d currently available - first is '%c' (%d)\n",
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
	    fprintf(stderr, "Got zero byte read with %d available\n", GetAvailable());
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
	int n = sink->WriteOut(output_buffer.data(), output_buffer.length());
	output_buffer.erase(0, n);
    }
}

void OutputStreamBuffer::Append(const BYTE *begin, int length)
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
	const BYTE *end = begin + length;
	while (begin < end)
	{
	    int n = sink->WriteOut(begin, length);
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
