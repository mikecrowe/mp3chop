#include <stdio.h>
#include "header.h"

const short MPEGHeader::bitrate_table[2][3][16] =
{
    // MPEG1
    {
	// Layer 1
	{
	    0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, -1
	},
	// Layer 2
	{
	    0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384, -1
	},
	// Layer 3
	{
	    0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, -1
	}
    },
    // MPEG2 & 2.5
    {
	// Layer 1
	{
	    0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256, -1
	},
	// Layer 2
	{
	    0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, -1
	},
	// Layer 3
	{
	    0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, -1
	}
    }
};

const int MPEGHeader::samplerate_table[4][4] =
{
    // MPEG Version 2.5
    {
	11025, 12000, 8000, -1
    },
    // Reserved
    {
	-1, -1, -1, -1
    },
    // MPEG Version 2
    {
	22050, 24000, 16000, -1
    },
    // MPEG Version 1
    {
	44100, 48000, 32000, -1
    }
};

void MPEGHeader::Dump()
{
    if (!Valid())
	printf("NOT VALID!!!!\n");
    fprintf(stderr, "  Word: %08x\n", word);
    fprintf(stderr, "  Sync: %d\n", IsSync());
    fprintf(stderr, "  CRC protected: %d\n", Protected());
    fprintf(stderr, "  Version: %s\n", VersionString());
    fprintf(stderr, "  Layer: %d\n", LayerNumber());
    fprintf(stderr, "  Bitrate: %d\n", Bitrate());
    fprintf(stderr, "  Samplerate: %d\n", SampleRate());
    fprintf(stderr, "  Padded: %d\n", Padded());
    fprintf(stderr, "  Private: %d\n", Private());
    fprintf(stderr, "  Channel mode: %s\n", ChannelModeName());
    fprintf(stderr, "  Copyright: %d\n", Copyright());
    fprintf(stderr, "  Original: %d\n", Original());
    fprintf(stderr, "  Emphasis: %s\n", EmphasisName());
    fprintf(stderr, "  Frame length: %d\n", FrameLength());
    fprintf(stderr, "  Data length: %d\n", DataLength());
}
