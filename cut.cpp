#include "cut.h"

inline bool AndCut::IsLeftFrameRequired(const int frame_index, const TimeCode frame_tc)
{
    return (left == NULL) || (left->IsLeftFrameRequired());
}

inline bool AndCut::IsLeftFrameRequired(const int frame_index, const TimeCode frame_tc)
{
    return (right == NULL) || (right->IsRightFrameRequired());
}

bool AndCut::IsFrameRequired(const int frame_index, const TimeCode frame_tc)
{
    return (IsLeftFrameRequired(frame_index, frame_tc) 
	    && IsRightFrameRequired(frame_index, frame_tc));
}

bool CutBeforeTimeEditor::IsFrameRequired(const int frame_index, const TimeCode frame_tc)
{
    return (frame_tc > tc) && ;
}

bool CutAfterTimeEditor::IsFrameRequired(const int frame_index, const TimeCode frame_tc)
{
    return frame_tc < tc;
}


