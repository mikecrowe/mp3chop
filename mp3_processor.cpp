#include "mp3_processor.h"
#include <cstdio>
#include <climits>
#include <unistd.h>
#include <vector>
#include "file_data_source.h"
#include "file_data_sink.h"
#include "xing_frame.h"

const int MAX_FRAME_LENGTH = 4096;

int MP3Processor::ConvertTimeCodeToFrameNumber(MPEGHeader *h, const TimeCode &tc)
{
    fprintf(stderr, "SampleRate=%d, tc_hundredths=%d, samples_per_frame=%d\n",
	    h->SampleRate(), tc.GetAsHundredths(), h->SamplesPerFrame());

    return ((h->SampleRate() * tc.GetAsHundredths()) / 100)/(h->SamplesPerFrame());
}

bool MP3Processor::IsID3Header(const BYTE *p)
{
    return (p[0] == 'T') && (p[1] == 'A') && (p[2] == 'G');
}

void MP3Processor::ProcessFrames(InputStreamBuffer *input, OutputStreamBuffer *output, Chop *chop, Filter *filter)
{
    //unsigned long header;
    MPEGHeader h;
    
    //    int start_frame = -1;
    //    int end_frame = -1;
    bool found_frame = false;
    int input_frame_number = 0;
    int output_frame_number = 0;
    bool found_sync = 0;
    int output_samples_per_frame = 0;
    int output_sample_rate = 0;
    bool xing_fixup_required = false;
    XingFrame xing;
    std::vector<int> frame_offsets;
    
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
		    fprintf(stderr, "Found sync at offset %d\n", input->GetOffset());
		}
		
		input->EnsureAvailable(h.FrameLength());

		// It is, so we can be pretty sure it is an OK frame.
		if (!found_frame)
		{
		    output_samples_per_frame = h.SamplesPerFrame();
			output_sample_rate = h.SampleRate();
		    
		    found_frame = true;
		}

		// Make sure the entire frame is available
		input->EnsureAvailable(h.FrameLength());

		if (xing.Read(input->GetPointer(), input->GetPointer() + h.FrameLength()))
		{
		    int frames;
		    if (xing.GetFrameCount(&frames))
		    {
			if (!frames)
			    xing_fixup_required = true;
		    }
		    output->SetBookmark();
		}

		if (h.SamplesPerFrame() != output_samples_per_frame)
		{
			fprintf(stderr, "Warning: output_samples_per_frame mismatch.\n");
		}
		// Process frame
		TimeCode current_time((static_cast<long long>(input_frame_number) * output_samples_per_frame * 100LL)
							  / static_cast<long long>(output_sample_rate));
		if (chop->IsFrameRequired(input_frame_number, current_time))
		{
		    if (filter)
		    {
			BYTE b[MAX_FRAME_LENGTH];
			unsigned l = h.FrameLength();
			memcpy(b, input->GetPointer(), h.FrameLength());
			filter->Apply(&h, b, h.FrameLength());
			h.Write(b);
			output->Append(b, h.FrameLength());
		    }
		    else
		    {
			output->Append(input->GetPointer(), h.FrameLength());
		    }
		    output_frame_number++;
		    frame_offsets.push_back(output->GetOffset());
		}
		
		// Now move past it.
		input->Advance(h.FrameLength());
		input_frame_number++;
		continue;
	    }
	    // OK, if we got here then it wasn't valid sync.
	    //BYTE b = *(input->GetPointer());
	    //fprintf(stderr, "Lost sync on character '%c' (%d)\n", isprint(b) ? b : '.', b);
	    
	    input->Advance(1);

	    if (found_sync)
		fprintf(stderr, "Lost sync at offset %d\n", input->GetOffset());
	    found_sync = false;
	}
    }
    catch (InsufficientDataException &)
    {
	fprintf(stderr, "End of file found at %d frames.\n", input_frame_number);
	// We've run out of data - that's fine, just suck up the remaining bytes
	while (input->GetAvailable())
	    input->Advance(1);

	if (xing_fixup_required)
	{
	    xing.SetFrameCount(output_frame_number);

	    int file_length = output->GetOffset();
	    double bit = frame_offsets.size()/100;
	    for(int i = 0; i < 100; ++i)
	    {
		int frame = i * static_cast<int>(bit);
		int offset = frame_offsets[frame];
		double fraction = (double)offset/(double)file_length;
		BYTE toc_entry = static_cast<BYTE>(fraction * 256);
		xing.SetTocEntry(i, toc_entry);
	    }
	    output->GoToBookmark();

	    BYTE *xing_frame;
	    int xing_frame_length;
	    if (xing.GetFrameContent(&xing_frame, &xing_frame_length))
	    {
		output->Append(xing_frame, xing_frame_length);
	    }
	}
	output->ClearBookmark();
    }
    catch (FileException &e)
    {
	std::cerr << "File error: " << e.Description() << std::endl;
    }
}

void MP3Processor::HandleID3Tag(InputStreamBuffer *input, OutputStreamBuffer *output)
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
	    output->Append(input->GetPointer(), 128);
	}
    }
    catch(InsufficientDataException &)
    {
	std::cerr << "Bad thing happened - less than 128 bytes there for ID3 checking." << std::endl;
    }
}

bool MP3Processor::GetFirstHeader(InputStreamBuffer *input, MPEGHeader *header)
{
    try
    {
	while (true)
	{
	    input->EnsureAvailable(4);
	    if (header->Read(input->GetPointer()))
	    {
		// This is sync, but is the next frame?
		input->EnsureAvailable(header->FrameLength() + 4);				
		
		MPEGHeader h2;
		if (h2.Read(input->GetPointer() + header->FrameLength()))
		{
		    return true;
		}
	    }
	    // Nope - we haven't found sync.
	    input->Advance(1);
	}
    }
    catch (InsufficientDataException &)
    {
	std::cerr << "Run out of data." << std::endl;
    }
    return false;
}

bool MP3Processor::DumpFirstHeader(DataSource *data_source)
{
    try
    {
	MPEGHeader header;

	InputStreamBuffer input(131072, 128);	
	input.SetSource(data_source);
	if (GetFirstHeader(&input, &header))
	{
	    header.Dump();
	    return true;
	}
    }
    catch (FileException &e)
    {
	std::cerr << "File error: " << e.Description() << std::endl;
    }
    return false;
}


bool MP3Processor::ProcessFile(DataSource *data_source, DataSink *data_sink, Chop *chop, Filter *filter)
{
#if 0
    fprintf(stderr, " Start time: %d:%02d:%02d.%02d\n",
	    begin_tc.GetHours(), begin_tc.GetMinutes(), begin_tc.GetSeconds(), begin_tc.GetHundredths());
    if (end_tc.GetAsHundredths())
    {
	fprintf(stderr, " End time: %d:%02d:%02d.%02d\n",
		end_tc.GetHours(), end_tc.GetMinutes(), end_tc.GetSeconds(), end_tc.GetHundredths());
    }
#endif
    try
    {
	InputStreamBuffer input(131072, 128);	
	input.SetSource(data_source);
	OutputStreamBuffer output;
	output.SetSink(data_sink);
	
	ProcessFrames(&input, &output, chop, filter);
	if (keep_id3)
	    HandleID3Tag(&input, &output);
	
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
	FileDataSource data_source;
	FileDataSink data_sink;
	
	if (file == "-")
	    data_source.OpenStandardInput();
	else
	    data_source.Open(file);

	if (m_mode == 'c')
	{
	    data_sink.OpenStandardOutput();

	    AndChop chop(begin_chop.get(), end_chop.get());
	    if (!ProcessFile(&data_source, &data_sink, &chop, scms_filter.get()))
	    {
		std::cerr << "Failed to process file \'" << file << "\'\n" << std::endl;
		exit(2);
	    }
	}
	else if (m_mode == 'd')
	{
	    if (!DumpFirstHeader(&data_source))
	    {
		std::cerr << "Failed to process file \'" << file << "\'\n" << std::endl;
		exit(2);
	    }
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

void MP3Processor::HandleMode(char mode)
{
    m_mode = mode;
}

void MP3Processor::HandleBeginTimeCode(const std::string &tc_str)
{
    TimeCode begin_tc;
    ParseTimeCode(&begin_tc, tc_str);
	auto_ptr<Chop> p(new BeforeTimeChop(begin_tc));
    begin_chop = p;
}

void MP3Processor::HandleEndTimeCode(const std::string &tc_str)
{
    TimeCode end_tc;
    ParseTimeCode(&end_tc, tc_str);
	auto_ptr<Chop> p(new AfterTimeChop(end_tc));
    end_chop = p;
}

void MP3Processor::CreateSCMSFilter()
{
    if (scms_filter.get() == NULL)
    {
	std::auto_ptr<SCMSFilter> p(new SCMSFilter);
	scms_filter = p;
    }
}

void MP3Processor::HandleForceCopyright(bool b)
{
    CreateSCMSFilter();
    scms_filter->SetCopyright(b);
    fprintf(stderr, "bing\n");
}

void MP3Processor::HandleForceOriginal(bool b)
{
    CreateSCMSFilter();
    scms_filter->SetOriginal(b);
    fprintf(stderr, "bong\n");
}
