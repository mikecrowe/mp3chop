#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string>
#include <errno.h>
#include <limits.h>
#include <ctype.h>

#define _GNU_SOURCE
#include <getopt.h>
#include "header.h"
#include "mp3_timecode.h"
#include "buffer.h"
#include "file_data_source.h"

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
    TimeCode begin_tc;
    TimeCode end_tc;
    int files;
    bool keep_id3;
    
    static void ParseTimeCode(TimeCode *tc, const std::string &t);
    static int ConvertTimeCodeToFrameNumber(MPEGHeader *h, const TimeCode &tc);
    static bool IsID3Header(const BYTE *p);
    
    bool ProcessFile(const TimeCode &begin_tc, const TimeCode &end_tc, DataSource *ds);
    void ProcessFrames(StreamBuffer *input, TimeCode start_tc, TimeCode end_tc);
    void HandleID3Tag(StreamBuffer *input);
    
public:
    MP3Processor() : begin_tc(0), end_tc(0), files(0) {};
    
    void HandleFile(const std::string &);
    void HandleBeginTimeCode(const std::string &);
    void HandleEndTimeCode(const std::string &);
    void SetKeepID3(bool b = true) { keep_id3 = b; }
    void HandleEnd();
};

int MP3Processor::ConvertTimeCodeToFrameNumber(MPEGHeader *h, const TimeCode &tc)
{
    return ((h->SampleRate() * tc.GetAsHundredths()) / 100)/(h->SamplesPerFrame());
}

bool MP3Processor::IsID3Header(const BYTE *p)
{
    return (p[0] == 'T') && (p[1] == 'A') && (p[2] == 'G');
}

void MP3Processor::ProcessFrames(StreamBuffer *input, TimeCode start_tc, TimeCode end_tc)
{
    //unsigned long header;
    MPEGHeader h;
    
    int start_frame = -1;
    int end_frame = -1;
    bool found_frame = false;
    int frame_number = 0;
    bool found_sync = 0;
    
    try
    {
	while (true)
	{
	    input->EnsureAvailable(4);
	    if (h.Read(input->GetPointer()))
	    {
		if (!found_sync)
		{
		    // This is sync, but is the next frame?
		    input->EnsureAvailable(h.FrameLength() + 4);				
		    
		    MPEGHeader h2;
		    if (!h2.Read(input->GetPointer() + h.FrameLength()))
		    {
			// Nope - we haven't found sync.
			input->Advance(1);
			continue;
		    }
		    found_sync = true;
		}
		
		input->EnsureAvailable(h.FrameLength());
		
		// It is, so we can be pretty sure it is an OK frame.
		if (!found_frame)
		{
		    start_frame = ConvertTimeCodeToFrameNumber(&h, start_tc);
		    end_frame = ConvertTimeCodeToFrameNumber(&h, end_tc);
		    
		    fprintf(stderr, "Start frame is %d\n", start_frame);
		    fprintf(stderr, "End frame is %d\n", end_frame);
		    if (end_frame == 0)
			end_frame = INT_MAX;
		    
		    found_frame = true;
		}
		
				// Process frame
		if (frame_number >= start_frame && frame_number < end_frame)
		{
		    //fprintf(stderr, "Outputting frame %d (length=%d)\n", frame_number, h.FrameLength());
		    write(1, input->GetPointer(), h.FrameLength());
		}
#if 0
		else
		    fprintf(stderr, "Skipping frame %d\n", frame_number);
#endif
		
				// Now move past it.
		input->Advance(h.FrameLength());
		frame_number++;
		continue;
	    }
	    // OK, if we got here then it wasn't valid sync.
	    BYTE b = *(input->GetPointer());
	    fprintf(stderr, "Lost sync on character '%c' (%d)\n", isprint(b) ? b : '.', b);
	    
	    input->Advance(1);
	    found_sync = false;
	}
    }
    catch (InsufficientDataException &)
    {
	// We've run out of data - that's fine, just suck up the remaining bytes
	while (input->GetAvailable())
	    input->Advance(1);
    }
    catch (FileException &e)
    {
	std::cerr << "File error: " << e.Description() << std::endl;
    }
}

void MP3Processor::HandleID3Tag(StreamBuffer *input)
{
    try
    {
	// We're at the end of the file - go 128 bytes back.
	input->Rewind(128);
	BYTE ch = *(input->GetPointer());
	fprintf(stderr, "Byte at -128 is %c (0x%x)\n", isprint(ch) ? ch : '.', ch);
	if (IsID3Header(input->GetPointer()))
	{
	    fprintf(stderr, "Found an ID3 tag.\n");
	    // We've got an ID3 header. Just output the block.
	    write(1, input->GetPointer(), 128);
	}
    }
    catch(InsufficientDataException &)
    {
	std::cerr << "Bad thing happened - less than 128 bytes there for ID3 checking." << std::endl;
    }
}



bool MP3Processor::ProcessFile(const TimeCode &begin_tc, const TimeCode &end_tc, DataSource *ds)
{
    fprintf(stderr, " Start time: %d:%02d:%02d.%02d\n",
	    begin_tc.GetHours(), begin_tc.GetMinutes(), begin_tc.GetSeconds(), begin_tc.GetHundredths());
    if (end_tc.GetAsHundredths())
    {
	fprintf(stderr, " End time: %d:%02d:%02d.%02d\n",
		end_tc.GetHours(), end_tc.GetMinutes(), end_tc.GetSeconds(), end_tc.GetHundredths());
    }
    
    try
    {
	StreamBuffer input(131072, 128);	
	input.SetSource(ds);
	
	ProcessFrames(&input, begin_tc, end_tc);
	if (keep_id3)
	    HandleID3Tag(&input);
	
	return true;
    }
    catch (FileException &e)
    {
	std::cerr << "File error: " << e.Description() << std::endl;
	return false;
    }
}

void MP3Processor::InvalidTimeCodeException::Report()
{
    std::cerr << "Invalid timecode specified: " << offendor << std::endl;
}

void MP3Processor::BadFileException::Report()
{
    std::cerr << "Unable to open file \'" << file << "\': " << error << std::endl;
}

void MP3Processor::ParseTimeCode(TimeCode *tc, const std::string &t)
{
    int m = 0;
    int s = 0;
    int f = 0;
    
    int c = sscanf(t.c_str(), "%d:%d.%d", &m, &s, &f);
    
    if (c > 2)
	tc->Set(0, m, s, f);
    else
	throw InvalidTimeCodeException(t);
}

void MP3Processor::HandleFile(const std::string &file)
{
    try
    {
	FileDataSource ds;
		
	if (file == "-")
	    ds.OpenStandardInput();
	else
	    ds.Open(file);
	
	if (!ProcessFile(begin_tc, end_tc, &ds))
	{
	    fprintf(stderr, "Failed to process file \'%s\'\n", optarg);
	    exit(2);
	}
	++files;
    }
    catch (FileException &e)
    {
	throw BadFileException(file, e.Description());
    }
}

void MP3Processor::HandleEnd()
{
    if (!files)
	HandleFile("-");
}

void MP3Processor::HandleBeginTimeCode(const std::string &tc_str)
{
    ParseTimeCode(&begin_tc, tc_str);
}

void MP3Processor::HandleEndTimeCode(const std::string &tc_str)
{
    ParseTimeCode(&end_tc, tc_str);
}

int main(int ac, char *av[])
{
    static struct option long_options[] =
    {
	{ "begin", 1, NULL, 'b' },
	{ "end", 1, NULL, 'e' },
	{ "id3", 0, NULL, 'i' },
	{ 0, 0, 0, 0 }
    };
    
    const char *short_options = "-b:e:i";
    
    MP3Processor processor;
    
    try
    {
	while (optind < ac)
	{
	    int option_index;
	    const int option = getopt_long(ac, av, short_options, long_options, &option_index);
	    
	    switch (option)
	    {
	    case 1:
		processor.HandleFile(optarg);
		break;
	    case 'b':
		processor.HandleBeginTimeCode(optarg);
		break;
	    case 'e':
		processor.HandleEndTimeCode(optarg);
		break;
	    case 'i':
		processor.SetKeepID3(true);
		break;
	    case '?':
		exit(1);
	    }
	}
	processor.HandleEnd();
    }
    catch (MP3Processor::Exception &e)
    {
	e.Report();
    }
}
