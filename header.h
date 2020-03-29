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

#ifndef HEADER_H
#define HEADER_H

#include "trace.h"
#include "types.h"

class MPEGHeader
{
    uint32_t word;
    uint32_t word2;
    uint16_t crc;
    
    // There are two different versions, three layers and sixteen bitrates.
    static const short bitrate_table[2][3][16];
    static const int samplerate_table[4][4];

    // MPEG1/MPEG2

    // AAAAAAAA AAABBCCD EEEEFFGH IIJJKLMM (QQQQQQQQ QQQQQQQQ)

    // MPEG4
    // AAAAAAAA AAAABCCD EEFFFFGH HHIJKLMM MMMMMMMM MMMOOOOO OOOOOOPP (QQQQQQQQ QQQQQQQQ)
    
    
public:
    // The maximum size of an MPEG header is four bytes of header and
    // two bytes of CRC.  The maximum size of an ADTS header is seven
    // bytes of header and two bytes of CRC.
    static const int MAX_HEADER_SIZE = 9;
    
    bool Read(const uint8_t *p)
    {
	// Read the word.
	uint8_t b[4];
	
	b[0] = *p++;
	b[1] = *p++;
	b[2] = *p++;
	b[3] = *p++;
	word = (b[0] << 24) | (b[1] << 16) | (b[2] << 8) | b[3];

	if (ADTS())
	{
	    word2 = (p[0] << 24) | (p[1] << 16) | (p[2] << 8);
	    p += 3;
	}
	else
	{
	    word2 = 0;
	}
	
	if (Protected())
	{
	    b[0] = *p++;
	    b[1] = *p++;
	    crc = (b[0] << 8) | b[1];
	}
	return IsSync() && Valid();
    }

    void Write(uint8_t *p)
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

    bool ADTS() const
    {
	// ADTS sync word is 12 bits and layer is always zero.
	return ((word & 0xfff60000) == 0xfff00000);
    }
    
    int MPEGVersionCode() const
    {
	return (word & 0x00180000) >> 19;
    }

    int ADTSVersionCode() const
    {
	return (word & 0x0008000) >> 20;
    }

    const char *VersionString() const
    {
	// Layer is always zero for ADTS but illegal for MPEG
	if (ADTS())
	{
	    switch (ADTSVersionCode())
	    {
	    case 0:
		return "MPEG 4/AAC";
	    case 1:
		return "MPEG Version 2?";
	    default:
		return "Unknown version";
	    }
	}
	else
	{
	    switch (MPEGVersionCode())
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
	if (ADTS())
	{
	    // TODO
	    return true;
	}
	else
	{
	    const int bitrate_code = MPEGBitrateCode();
	    return (bitrate_code != 0xf)
		&& (bitrate_code != 0x0)
		&& (MPEGSampleRateCode() != 0x3)
		&& (MPEGVersionCode() != 1)
		&& (LayerCode() != 0);
	}
    }
    
    bool Protected() const
    {
	return ((word & 0x00010000) >> 16) == 0;
    }

    unsigned int ADTSAudioObjectType() const
    {
	return ((word & 0x0000c000) >> 14) + 1;
    }

    const char *ADTSAudioObjectTypeString() const
    {
	switch (ADTSAudioObjectType())
	{
	case 1:
	    return "AAC Main";
	case 2:
	    return "AAC LC";
	case 3:
	    return "AAC SSR";
	case 4:
	    return "AAC LTP";
	default:
	    return "Invalid";
	}
    }

    int ADTSSamplingFrequencyIndex() const
    {
	return ((word & 0x00003c00) >> 10);
    }

    int ADTSSamplingFrequency() const
    {
	switch (ADTSSamplingFrequencyIndex())
	{
	case 0:
	    return 96000;
	case 1:
	    return 88200;
	case 2:
	    return 64000;
	case 3:
	    return 48000;
	case 4:
	    return 44100;
	case 5:
	    return 32000;
	case 6:
	    return 24000;
	case 7:
	    return 22050;
	case 8:
	    return 16000;
	case 9:
	    return 12000;
	case 10:
	    return 11025;
	case 11:
	    return 8000;
	case 12:
	    return 7350;
	default:
	    return -1;
	}
    }

    int ADTSPrivateStream() const
    {
	return ((word & 0x00000200) >> 9);
    }

    int ADTSChannelConfiguration() const
    {
	return ((word & 0x000001c0) >> 6);
    }

    const char *ADTSChannelConfigurationString() const
    {
	switch (ADTSChannelConfiguration())
	{
	case 0:
	    return "Specified elsewhere";
	case 1:
	    return "Mono front-center";
	case 2:
	    return "Stereo front-left, front-right";
	case 3:
	    return "front-center, front-left, front-right";
	case 4:
	    return "front-center, front-left, front-right, back-center";
	case 5:
	    return "front-center, front-left, front-right, back-left, back-right";
	case 6:
	    return "front-center, front-left, front-right, back-left, back-right, LFE";
	case 7:
	    return "front-center, front-left, front-right, side-left, side-right, back-left, back-right, LFE";
	default:
	    return "Unknown";
	}
    }

    int ADTSOriginality() const
    {
	return ((word & 0x00000020) >> 5);
    }

    void ADTSSetOriginality(bool b)
    {
	if (b)
	    word |= 0x20;
	else
	    word &= ~0x20;
    }

    int ADTSHome() const
    {
	return ((word & 0x00000010) >> 4);
    }

    int ADTSCopyrightedStream() const
    {
	return ((word & 0x00000008) >> 3);
    }

    void ADTSSetCopyrightedStream(bool b)
    {
	if (b)
	    word |= 0x8;
	else
	    word &= ~0x8;
    }

    int ADTSCopyrightStart() const
    {
	return ((word & 0x00000004) >> 2);
    }

    int ADTSFrameLength() const
    {
	return ((word & 0x00000002) << 11) | (word2 >> 21);
    }

    int ADTSBufferFullness() const
    {
	return ((word2 & 0x001fc000) >> 10);
    }

    int ADTSAACFrames() const
    {
	return ((word2 & 0x0000300) >> 8) + 1;
    }
    
    int MPEGBitrateCode() const
    {
	return ((word & 0x0000f000) >> 12);
    }
    
    int MPEGBitrate() const
    {
	const int version_code = MPEGVersionCode();
	const int version_index = (version_code == 3) ? 0 : 1;
	const int layer_index = LayerNumber() - 1;
	const int bitrate_index = MPEGBitrateCode();

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
    
    int MPEGSampleRateCode() const
    {
	return ((word & 0x00000C00) >> 10);
    }
    
    int MPEGSampleRate() const
    {
	const int version_code = MPEGVersionCode();
	const int samplerate_code = MPEGSampleRateCode();

	ASSERT(version_code >= 0);
	ASSERT(version_code < 4);
	ASSERT(samplerate_code >= 0);
	ASSERT(samplerate_code < 4);
	
	const int result = samplerate_table[version_code][samplerate_code];
	ASSERT(result > 0);
	return result;
    }

    int SampleRate() const
    {
	if (ADTS())
	    return ADTSSamplingFrequency();
	else
	    return MPEGSampleRate();
    }
    
    bool MPEGPadded() const
    {
	return ((word & 0x00000200) >> 9) != 0;
    }

    bool MPEGPrivate() const
    {
	return ((word & 0x00000100) >> 8) != 0;
    }
    
    int MPEGChannelModeCode() const
    {
	return ((word & 0x000000c0) >> 6);
    }
    
    const char *MPEGChannelModeName() const
    {
	switch (MPEGChannelModeCode())
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
    
    bool MPEGCopyright() const
    {
	return ((word & 0x00000008) >> 3) != 0;
    }

    void MPEGSetCopyright(bool b)
    {
	if (b)
	    word |= 0x8;
	else
	    word &= ~0x8;
    }

    bool Copyright() const
    {
	if (ADTS())
	    return ADTSCopyrightedStream();
	else
	    return MPEGCopyright();
    }

    void SetCopyright(bool b)
    {
	if (ADTS())
	    ADTSSetCopyrightedStream(b);
	else
	    MPEGSetCopyright(b);
    }
    
    bool Original() const
    {
	if (ADTS())
	    return ADTSOriginality();
	else
	    return MPEGOriginality();
    }

    bool MPEGOriginality() const
    {
	return ((word & 0x00000004) >> 2) != 0;
    }

    void MPEGSetOriginal(bool b)
    {
	if (b)
	    word |= 0x4;
	else
	    word &= ~0x4;
    }

    void SetOriginal(bool b)
    {
	if (ADTS())
	    ADTSSetOriginality(b);
	else
	    MPEGSetOriginal(b);
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
    
    int MPEGFrameLength() const
    {
	int result;
	if (LayerNumber() == 1)
	{
	    // MPEG 1, 2 and 2.5 layer 1
	    result = (12 * MPEGBitrate() / MPEGSampleRate() + (MPEGPadded() ? 1: 0)) * 4;
	}
	else if (MPEGVersionCode() == 3)
	{
	    // MPEG 1 layers 2 & 3
	    result = 144 * MPEGBitrate() / MPEGSampleRate() + MPEGPadded();
	}
	else
	{
	    // MPEG 2 & 2.5 layers 2 & 3
	    // This is a bit of a dubious fix but it makes certain files work!
	    result = 72 * MPEGBitrate() / MPEGSampleRate() + MPEGPadded();
	}
	ASSERT(result > 0);
	return result;
    }

    int FrameLength() const
    {
	if (ADTS())
	    return ADTSFrameLength();
	else
	    return MPEGFrameLength();
    }
    
    int DataLength() const
    {
	if (ADTS())
	    return ADTSFrameLength() - 7 - ProtectionLength();
	else
	    return MPEGFrameLength() - 4 - ProtectionLength();
    }
    
    int SamplesPerFrame() const
    {
	if (ADTS())
	{
	    return 1024;
	}
	else if (LayerNumber() == 1)
	{
	    return 384;
	}
	else if (MPEGVersionCode() == 3)
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
