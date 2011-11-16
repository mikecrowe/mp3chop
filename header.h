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

#ifndef HEADER_H
#define HEADER_H

#include "trace.h"
#include "types.h"

class MPEGHeader
{
    int word;
    short crc;
    
    // There are two different versions, three layers and sixteen bitrates.
    static const short bitrate_table[2][3][16];
    static const int samplerate_table[4][4];
    
public:
    // The maximum size of an MPEG header is four bytes of header and
    // two bytes of CRC.
    static const int MAX_HEADER_SIZE = 6;
    
    bool Read(const BYTE *p)
    {
	// Read the word.
	BYTE b[4];
	
	b[0] = *p++;
	b[1] = *p++;
	b[2] = *p++;
	b[3] = *p++;
	word = (b[0] << 24) | (b[1] << 16) | (b[2] << 8) | b[3];
	
	if (Protected())
	{
	    b[0] = *p++;
	    b[1] = *p++;
	    crc = (b[0] << 8) | b[1];
	}
	return IsSync() && Valid();
    }

    void Write(BYTE *p)
    {
	*p++ = (word >> 24);
	*p++ = (word >> 16) & 0xff;
	*p++ = (word >> 8) & 0xff;
	*p++ = word & 0xff;
    }
    
    bool IsSync() const
    {
	return (word & 0xFFE00000) == 0xFFE00000;
    }
    
    int VersionCode() const
    {
	return (word & 0x00180000) >> 19;
    }
    
    const char *VersionString() const
    {
	switch (VersionCode())
	{
	case 0:
	    return "MPEG Version 2.5";
	case 1:
	    return "Illegal";
	case 2:
	    return "MPEG Version 2";
	case 3:
	    return "MPEG Version 1";
	default:
	    return "Unknown version";
	}
    }
    
    int LayerCode() const
    {
	return (word & 0x00060000) >> 17;
    }
    
    int LayerNumber() const
    {
	switch (LayerCode())
	{
	case 0:
	    return 0;
	case 1:
	    return 3;
	case 2:
	    return 2;
	case 3:
	    return 1;
	default:
	    ASSERT(false);
	    return -1;
	}
    }
    
    bool Valid() const
    {
	const int bitrate_code = BitrateCode();
	return (bitrate_code != 0xf)
	    && (bitrate_code != 0x0)
	    && (SampleRateCode() != 0x3)
	    && (VersionCode() != 1)
	    && (LayerCode() != 0);
    }
    bool Protected() const
    {
	return ((word & 0x00010000) >> 16) == 0;
    }
    
    int BitrateCode() const
    {
	return ((word & 0x0000f000) >> 12);
    }
    
    int Bitrate() const
    {
	const int version_code = VersionCode();
	const int version_index = (version_code == 3) ? 0 : 1;
	const int layer_index = LayerNumber() - 1;
	const int bitrate_index = BitrateCode();

	ASSERT(version_index >= 0);
	ASSERT(version_index <= 1);
	ASSERT(layer_index >= 0);
	ASSERT(layer_index < 3);
	ASSERT(bitrate_index > 0);
	ASSERT(bitrate_index < 15);
	const int result = static_cast<int>(bitrate_table[version_index][layer_index][bitrate_index]) * 1000;
	ASSERT(result > 0);
	return result;
    }
    
    int SampleRateCode() const
    {
	return ((word & 0x00000C00) >> 10);
    }
    
    int SampleRate() const
    {
	const int version_code = VersionCode();
	const int samplerate_code = SampleRateCode();

	ASSERT(version_code >= 0);
	ASSERT(version_code < 4);
	ASSERT(samplerate_code >= 0);
	ASSERT(samplerate_code < 4);
	
	const int result = samplerate_table[version_code][samplerate_code];
	ASSERT(result > 0);
	return result;
    }
    
    bool Padded() const
    {
	return ((word & 0x00000200) >> 9) != 0;
    }

    bool Private() const
    {
	return ((word & 0x00000100) >> 8) != 0;
    }
    
    int ChannelModeCode() const
    {
	return ((word & 0x000000c0) >> 6);
    }
    
    const char *ChannelModeName() const
    {
	switch (ChannelModeCode())
	{
	case 0:
	    return "stereo";
	case 1:
	    return "joint stereo";
	case 2:
	    return "dual channel";
	case 3:
	    return "mono";
	default:
	    return "Unknown channel mode";
	}
    }
    
    bool Copyright() const
    {
	return ((word & 0x00000008) >> 3) != 0;
    }

    void SetCopyright(bool b)
    {
	if (b)
	    word |= 0x8;
	else
	    word &= ~0x8;
    }
    
    bool Original() const
    {
	return ((word & 0x00000004) >> 2) != 0;
    }

    void SetOriginal(bool b)
    {
	if (b)
	    word |= 0x4;
	else
	    word &= ~0x4;
    }
    
    int EmphasisCode() const
    {
	return (word & 0x00000003);
    }
    
    const char *EmphasisName() const
    {
	switch (EmphasisCode())
	{
	case 0:
	    return "none";
	case 1:
	    return "50/15ms";
	case 2:
	    return "reserved";
	case 3:
	    return "CCIT J.17";
	default:
	    return "Unknown emphasis";
	}
    }
    
    int ProtectionLength() const
    {
	return Protected() ? 2 : 0;
    }
    
    int FrameLength() const
    {
	int result;
	if (LayerNumber() == 1)
	{
	    // MPEG 1, 2 and 2.5 layer 1
	    result = (12 * Bitrate() / SampleRate() + (Padded() ? 1: 0)) * 4;
	}
	else if (VersionCode() == 3)
	{
	    // MPEG 1 layers 2 & 3
	    result = 144 * Bitrate() / SampleRate() + Padded();
	}
	else
	{
	    // MPEG 2 & 2.5 layers 2 & 3
	    // This is a bit of a dubious fix but it makes certain files work!
	    result = 72 * Bitrate() / SampleRate() + Padded();
	}
	ASSERT(result > 0);
	return result;
    }
    
    int DataLength() const
    {
	return FrameLength() - 4;
    }
    
    int SamplesPerFrame() const
    {
	if (LayerNumber() == 1)
	{
	    return 384;
	}
	else if (VersionCode() == 3)
	{
	    return 1152;
	}
	else
	{
	    // See FrameLength() above.
	    return 1152/2;
	}
    }
    
    void Dump();
};

#endif
