#ifndef BUFFER_H
#define BUFFER_H 1

#include "types.h"

class InsufficientDataException
{
};

class DataSource
{
public:
    virtual int ReadInto(BYTE *buffer, int bytes_required) = 0;
};

class StreamBuffer
{
    DataSource *source;
    BYTE *input_buffer;
    int input_size;
    int input_min;
    int input_writep;
    int input_readp;
    
protected:
    void ShoveUp();
    int Space() const;
    
public:
    StreamBuffer(int size, int lookbehind);
    ~StreamBuffer();
    DataSource *SetSource(DataSource *new_source)
    {
	DataSource *old_source = source;
	source = new_source;
	return old_source;
    }
    
    void EnsureAvailable(int count);
    const BYTE *GetPointer() const
    {
	return input_buffer + input_readp;
    }
    int GetAvailable() const;
    void Advance(int count);
    void Rewind(int count);
    
    bool IsEOFAt(int count);
};

#endif
