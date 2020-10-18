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

#include "chop.h"

inline bool AndChop::IsLeftFrameRequired(const int frame_index, const TimeCode frame_tc)
{
    return (left == 0) || (left->IsFrameRequired(frame_index, frame_tc));
}

inline bool AndChop::IsRightFrameRequired(const int frame_index, const TimeCode frame_tc)
{
    return (right == 0) || (right->IsFrameRequired(frame_index, frame_tc));
}

bool AndChop::IsFrameRequired(const int frame_index, const TimeCode frame_tc)
{
    return (IsLeftFrameRequired(frame_index, frame_tc)
            && IsRightFrameRequired(frame_index, frame_tc));
}

bool BeforeTimeChop::IsFrameRequired(const int frame_index, const TimeCode frame_tc)
{
    return (frame_tc > cut_time);
}

bool AfterTimeChop::IsFrameRequired(const int frame_index, const TimeCode frame_tc)
{
    return frame_tc < cut_time;
}

bool NullChop::IsFrameRequired(const int frame_index, const TimeCode frame_tc)
{
    return false;
}
