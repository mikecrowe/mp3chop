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


