#include <string.h>
#include "buffer.h"
#include <stdio.h>
#include <ctype.h>

StreamBuffer::StreamBuffer(int size, int lookbehind)
	: source(0), input_size(size), input_min(lookbehind), input_writep(0), input_readp(0)
{
	input_buffer = reinterpret_cast<BYTE *>(operator new(size));
}

StreamBuffer::~StreamBuffer()
{
	delete input_buffer;
}

void StreamBuffer::ShoveUp()
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

int StreamBuffer::Space() const
{
	return input_size - input_writep;
}

int StreamBuffer::GetAvailable() const
{
	return input_writep - input_readp;
}

void StreamBuffer::EnsureAvailable(int count)
{
	if (GetAvailable() < count)
	{
		ShoveUp();

		input_writep += source->ReadInto(input_buffer + input_writep, Space());
		
		if (GetAvailable() < count)
			throw InsufficientDataException();
	}
}

void StreamBuffer::Advance(int count)
{
	input_readp += count;
}						

void StreamBuffer::Rewind(int count)
{
	if (input_readp >= count)
		input_readp -= count;
	else
		throw InsufficientDataException();
}

// Keep reading data until either we have _more_ than count or we find
// EOF.
bool StreamBuffer::IsEOFAt(int count)
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
	
