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

    if (ADTS())
    {
        printf("  Word: %08x %06x\n", word, word2 >> 8);
        printf("  Sync: %d\n", IsSync());
        printf("  CRC protected: %d\n", Protected());
        printf("  Version: %s\n", VersionString());
        printf("  Type: %s\n", ADTSAudioObjectTypeString());
        printf("  Samplerate: %d\n", ADTSSamplingFrequency());
        printf("  Private: %d\n", ADTSPrivateStream());
        printf("  Channel mode: %s\n", ADTSChannelConfigurationString());
        printf("  Copyright: %d\n", ADTSCopyrightedStream());
        printf("  Original: %d\n", ADTSOriginality());
        printf("  Frame length: %d\n", FrameLength());
        printf("  Data length: %d\n", DataLength());
        printf("  Samples per frame: %d\n", SamplesPerFrame());
    }
    else
    {
        printf("  Word: %08x\n", word);
        printf("  Sync: %d\n", IsSync());
        printf("  CRC protected: %d\n", Protected());
        printf("  Version: %s\n", VersionString());
        printf("  Layer: %d\n", LayerNumber());
        printf("  Bitrate: %d\n", MPEGBitrate());
        printf("  Samplerate: %d\n", SampleRate());
        printf("  Padded: %d\n", MPEGPadded());
        printf("  Private: %d\n", MPEGPrivate());
        printf("  Channel mode: %s\n", MPEGChannelModeName());
        printf("  Copyright: %d\n", Copyright());
        printf("  Original: %d\n", Original());
        printf("  Emphasis: %s\n", EmphasisName());
        printf("  Frame length: %d\n", MPEGFrameLength());
        printf("  Data length: %d\n", DataLength());
        printf("  Samples per frame: %d\n", SamplesPerFrame());
    }
}
