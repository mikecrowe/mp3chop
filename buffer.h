#ifndef BUFFER_H
#define BUFFER_H 1

#include "types.h"
#include <string>

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

class DataSink
{
public:
    virtual int WriteOut(const BYTE *buffer, int bytes_available) = 0;
};

class OutputStreamBuffer
{
    DataSink *sink;
    std::basic_string<BYTE> output_buffer;
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
