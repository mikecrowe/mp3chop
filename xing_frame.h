#ifndef XING_FRAME_H
#define XING_FRAME_H 1

#include "types.h"
#include <stdio.h>

class XingFrame
{
    enum
    {
	FRAMES_FLAG     = 0x0001,
	BYTES_FLAG      = 0x0002,
	TOC_FLAG        = 0x0004,
	VBR_SCALE_FLAG  = 0x0008,

	FRAMES_AND_BYTES= (FRAMES_FLAG | BYTES_FLAG),
    };
    int h_id;       // from MPEG header, 0=MPEG2, 1=MPEG1
    int samprate;   // determined from MPEG header
    int flags;      // from Xing header data
    int frames;     // total bit stream frames from Xing header data
    int bytes;      // total bit stream bytes from Xing header data
    int vbr_scale;  // encoded vbr scale from Xing header data
    unsigned char toc[100];
    BYTE *frame_content;
    int frame_content_length;

    int frames_pos;
    int toc_pos;
    
public:
    XingFrame();
    ~XingFrame();
    
    bool Read(const BYTE *begin, const BYTE *end);

    bool IsVBR() const;

    bool GetFrameCount(int *f) const
    {
	*f = frames;
	return (flags & FRAMES_FLAG);
    }
    bool GetByteCount(int *b) const
    {
	*b = bytes;
	return (flags & BYTES_FLAG);
    }
    void SetFrameCount(int f);
    void SetTocEntry(int i, BYTE value);

    bool GetFrameContent(BYTE **content, int *length)
    {
	*content = frame_content;
	*length = frame_content_length;
	fprintf(stderr, "%p:frame_content=%p\n", this, frame_content);
	return (frame_content != 0);
    }
};

#endif

