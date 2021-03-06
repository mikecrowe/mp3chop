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

#include "config.h"
#include "xing_frame.h"
#include <string.h>

inline int ExtractI4(const uint8_t *p)
{
    int x;
    // big endian extract

    x = p[0];
    x <<= 8;
    x |= p[1];
    x <<= 8;
    x |= p[2];
    x <<= 8;
    x |= p[3];

    return x;
}

inline void WriteI4(uint8_t *p, int i)
{
    *p++ = (i >> 24) & 0xff;
    *p++ = (i >> 16) & 0xff;
    *p++ = (i >> 8) & 0xff;
    *p++ = (i & 0xff);
}

XingFrame::XingFrame()
    : flags(0), frame_content(0), frame_content_length(0), frames_pos(0)
{
}

XingFrame::~XingFrame()
{
    delete frame_content;
    frame_content = 0;
}

bool XingFrame::Read(const uint8_t *begin, const uint8_t *end)
{
    int i, head_flags;
    int h_id, h_mode, h_sr_index;
    static const int sr_table[4] = { 44100, 48000, 32000, 99999 };

    // get Xing header data
    this->flags = 0;     // clear to null incase fail
    const uint8_t *buf = begin;

    // get selected MPEG header data
    h_id       = (buf[1] >> 3) & 1;
    h_sr_index = (buf[2] >> 2) & 3;
    h_mode     = (buf[3] >> 6) & 3;


    // determine offset of header
    if (h_id)
    {
        // mpeg1
        if (h_mode != 3 )
            buf+=(32+4);
        else
            buf+=(17+4);
    }
    else
    {
        // mpeg2
        if (h_mode != 3)
            buf+=(17+4);
        else
            buf+=(9+4);
    }

    if (buf[0] != 'X') return 0;    // fail
    if (buf[1] != 'i') return 0;    // header not found
    if (buf[2] != 'n') return 0;
    if (buf[3] != 'g') return 0;
    buf+=4;

    this->h_id = h_id;
    this->samprate = sr_table[h_sr_index];
    if (h_id == 0)
        this->samprate >>= 1;

    head_flags = this->flags = ExtractI4(buf); buf+=4;      // get flags

    if (head_flags & FRAMES_FLAG)
    {
        this->frames_pos = buf - begin;
        this->frames = ExtractI4(buf);
        buf+=4;
    }
    if (head_flags & BYTES_FLAG)
    {
        this->bytes = ExtractI4(buf);
        buf+=4;
    }

    if (head_flags & TOC_FLAG)
    {
        this->toc_pos = buf - begin;
        for(i=0;i<100;i++)
            this->toc[i] = buf[i];
        buf+=100;
    }

    this->vbr_scale = -1;
    if (head_flags & VBR_SCALE_FLAG)
    {
        this->vbr_scale = ExtractI4(buf);
        buf+=4;
    }

    delete frame_content;
    frame_content = 0;

    // Now save it all in frame_content.
    frame_content_length = buf - begin;
    frame_content = reinterpret_cast<uint8_t *>(operator new(frame_content_length));
    memcpy(frame_content, begin, frame_content_length);
    return 1;       // success
}

void XingFrame::SetFrameCount(int frame_count)
{
    WriteI4(frame_content + frames_pos, frame_count);
}

void XingFrame::SetTocEntry(int i, uint8_t v)
{
    *(frame_content + toc_pos + i) = v;
}
