/*
 * mp3chop
 *
 * (C) 2000-2010 Mike Crowe <mac@mcrowe.com>
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
    uint8_t *frame_content;
    int frame_content_length;

    int frames_pos;
    int toc_pos;
    
public:
    XingFrame();
    ~XingFrame();
    
    bool Read(const uint8_t *begin, const uint8_t *end);

    bool IsVBR() const;

    bool GetFrameCount(int *f) const
    {
	*f = frames;
	return (flags & FRAMES_FLAG);
    }
    bool GetByteCount(int *b) const
    {
	*b = bytes;
	return (flags & BYTES_FLAG) != 0;
    }
    void SetFrameCount(int f);
    void SetTocEntry(int i, uint8_t value);

    bool GetFrameContent(uint8_t **content, int *length)
    {
	*content = frame_content;
	*length = frame_content_length;
	fprintf(stderr, "%p:frame_content=%p\n", this, frame_content);
	return (frame_content != 0);
    }
};

#endif

