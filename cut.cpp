#include "cut.h"

inline bool AndCut::IsLeftFrameRequired(const int frame_index, const TimeCode frame_tc)
{
    return (left == 0) || (left->IsFrameRequired(frame_index, frame_tc));
}

inline bool AndCut::IsRightFrameRequired(const int frame_index, const TimeCode frame_tc)
{
    return (right == 0) || (right->IsFrameRequired(frame_index, frame_tc));
}

bool AndCut::IsFrameRequired(const int frame_index, const TimeCode frame_tc)
{
    return (IsLeftFrameRequired(frame_index, frame_tc) 
	    && IsRightFrameRequired(frame_index, frame_tc));
}

bool BeforeTimeCut::IsFrameRequired(const int frame_index, const TimeCode frame_tc)
{
    return (frame_tc > cut_time);
}

bool AfterTimeCut::IsFrameRequired(const int frame_index, const TimeCode frame_tc)
{
    return frame_tc < cut_time;
}


