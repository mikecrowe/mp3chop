#ifndef MP3_PROCESSOR_H
#define MP3_PROCESSOR_H 1

#include <string>
#include <memory>
#include "mp3_timecode.h"
#include "header.h"
#include "buffer.h"
#include "chop.h"

class MP3Processor
{
public:
    struct Exception
    {
	virtual void Report() = 0;
    };
    class InvalidTimeCodeException : public Exception
    {
	const std::string offendor;
    public:
	virtual ~InvalidTimeCodeException() {}
	InvalidTimeCodeException(const std::string &tc) : offendor(tc) {}
	virtual void Report();
    };
    class BadFileException : public Exception
    {
	const std::string file;
	const std::string error;
    public:
	BadFileException(const std::string &f, const std::string &e)
	    : file(f), error(e) {}
	virtual ~BadFileException() {}
	virtual void Report();
    };
    
private:
    std::auto_ptr<Chop> begin_chop;
    std::auto_ptr<Chop> end_chop;
    int files;
    char m_mode;
    bool keep_id3;
    
    static void ParseTimeCode(TimeCode *tc, const std::string &t);
    static int ConvertTimeCodeToFrameNumber(MPEGHeader *h, const TimeCode &tc);
    static bool IsID3Header(const BYTE *p);
    
    bool ProcessFile(DataSource *data_source, DataSink *data_sink, Chop *chop);
    void ProcessFrames(InputStreamBuffer *input, OutputStreamBuffer *output, Chop *chop);
    void HandleID3Tag(InputStreamBuffer *input, OutputStreamBuffer *output);
    bool GetFirstHeader(InputStreamBuffer *input, MPEGHeader *header);
    bool DumpFirstHeader(DataSource *data_source);
    
public:
    MP3Processor() : files(0), m_mode('c') {};
    
    void HandleFile(const std::string &);
    void HandleBeginTimeCode(const std::string &);
    void HandleEndTimeCode(const std::string &);
    void SetKeepID3(bool b = true) { keep_id3 = b; }
    void HandleEnd();
    void HandleMode(char);
};

#endif
