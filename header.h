#ifndef HEADER_H
#define HEADER_H

#include "types.h"

class MPEGHeader
{
    int word;
    short crc;
    
    // There are two different versions, three layers and sixteen bitrates.
    static const short bitrate_table[2][3][16];
    static const int samplerate_table[4][4];
    
public:
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
	return IsSync();
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
	}
    }
    
    bool Valid() const
    {
	return (BitrateCode() != 0xf)
	    && (SampleRateCode() != 0x3)
	    && (VersionCode() != 1);
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
	
	return static_cast<int>(bitrate_table[version_index][layer_index][bitrate_index]) * 1000;
    }
    
    int SampleRateCode() const
    {
	return ((word & 0x00000C00) >> 10);
    }
    
    int SampleRate() const
    {
	const int version_code = VersionCode();
	const int samplerate_code = SampleRateCode();
	
	return samplerate_table[version_code][samplerate_code];
    }
    
    bool Padded() const
    {
	return ((word & 0x00000200) >> 9);
    }

    bool Private() const
    {
	return ((word & 0x00000100) >> 8);
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
	}
    }
    
    bool Copyright() const
    {
	return ((word & 0x00000008) >> 3);
    }
    
    bool Original() const
    {
	return ((word & 0x00000004) >> 2);
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
	}
    }
    
    int ProtectionLength() const
    {
	return Protected() ? 2 : 0;
    }
    
    int FrameLength() const
    {
	if (LayerNumber() == 1)
	{
	    return (12 * Bitrate() / SampleRate() + (Padded() ? 1: 0)) * 4;
	}
	else
	{
	    return 144 * Bitrate() / SampleRate() + Padded();
	}
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
	else
	{
	    return 1152;
	}
    }
    
    void Dump()
    {
	if (!Valid())
	    printf("NOT VALID!!!!\n");
	printf("Word: %08x\n", word);
	printf("Sync: %d\n", IsSync());
	printf("CRC protected: %d\n", Protected());
	printf("Version: %s\n", VersionString());
	printf("Layer: %d\n", LayerNumber());
	printf("Bitrate: %d\n", Bitrate());
	printf("Samplerate: %d\n", SampleRate());
	printf("Padded: %d\n", Padded());
	printf("Private: %d\n", Private());
	printf("Channel mode: %s\n", ChannelModeName());
	printf("Copyright: %d\n", Copyright());
	printf("Original: %d\n", Original());
	printf("Emphasis: %s\n", EmphasisName());
	printf("Frame length: %d\n", FrameLength());
	printf("Data length: %d\n", DataLength());
    }
};

#endif
