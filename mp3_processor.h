/*
 * mp3chop
 *
 * (C) 2000-2020 Mike Crowe <mac@mcrowe.com>
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

#ifndef MP3_PROCESSOR_H
#define MP3_PROCESSOR_H 1

#include <string>
#include <memory>
#include "mp3_timecode.h"
#include "header.h"
#include "buffer.h"
#include "chop.h"
#include "filter.h"

class MalformedID3V2Exception
{
};

class UnsupportedID3V2Version
{
};

class MP3Processor
{
public:
    class Exception
    {
    protected:
        virtual ~Exception() {}
    public:
	virtual void Report() = 0;
    };
    class InvalidTimeCodeException : public Exception
    {
	const std::string offender;
    public:
	virtual ~InvalidTimeCodeException() {}
	InvalidTimeCodeException(const std::string &tc) : offender(tc) {}
	virtual void Report();
    };
    
    class BadFileException : public Exception
    {
	const std::string m_file;
	const std::string m_error;
    public:
	BadFileException(const std::string &f, const std::string &e)
	    : m_file(f), m_error(e) {}
	virtual ~BadFileException() {}
	virtual void Report();
    };
    
private:
    std::unique_ptr<Chop> begin_chop;
    std::unique_ptr<Chop> end_chop;
    std::unique_ptr<SCMSFilter> scms_filter;
    
    int m_files;
    char m_mode;
    bool m_keep_id3v1;
    bool m_keep_id3v2;
    
    static void ParseTimeCode(TimeCode *tc, const std::string &t);
    static int ConvertTimeCodeToFrameNumber(MPEGHeader *h, const TimeCode &tc);

    static bool IsID3V1Header(const uint8_t *p);

    /** Only requires three bytes */
    static bool IsID3V2Header(const uint8_t *p);
    
    bool ProcessFile(DataSource *data_source, DataSink *data_sink, Chop *chop, Filter *filter);
    void ProcessFrames(InputStreamBuffer *input, OutputStreamBuffer *output, Chop *chop, Filter *filter);
    void HandleID3V1Tag(InputStreamBuffer *input, OutputStreamBuffer *output);
    bool HandleID3V2Tag(InputStreamBuffer *input, OutputStreamBuffer *output);
    bool GetFirstHeader(InputStreamBuffer *input, MPEGHeader *header);
    bool DumpFirstHeader(DataSource *data_source);
    void CreateSCMSFilter();
    
public:
    MP3Processor() : m_files(0), m_mode('c'), m_keep_id3v1(false) {};
    
    void HandleFile(const std::string &);
    void HandleBeginTimeCode(const std::string &);
    void HandleEndTimeCode(const std::string &);
    void SetKeepID3V1(bool b = true) { m_keep_id3v1 = b; }
    void SetKeepID3V2(bool b = true) { m_keep_id3v2 = b; }
    void HandleEnd();
    void HandleMode(char);
    void HandleForceCopyright(bool b);
    void HandleForceOriginal(bool b);
};

#endif
