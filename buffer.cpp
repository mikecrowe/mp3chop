#include <string.h>
#include "buffer.h"

StreamBuffer::StreamBuffer(int size)
	: source(0), input_size(size), input_writep(0), input_readp(0)
{
	input_buffer = reinterpret_cast<BYTE *>(operator new(size));
}

StreamBuffer::~StreamBuffer()
{
	delete input_buffer;
}

void StreamBuffer::ShoveUp()
{
	// if it's worth doing push everything up to the start of the buffer leaving space at
	// the end for new data.
	if (input_readp > 0)
	{
		int bytes_kept = input_writep - input_readp;
		if (bytes_kept)
			memmove(input_buffer, input_buffer + input_readp, bytes_kept);
		input_readp = 0;
		input_writep = bytes_kept;
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

