#ifndef EDITOR_H
#define EDITOR_H

#include "mp3_timecode.h"

class Cut
{
 public:
    virtual bool IsFrameRequired(const int frame_index, const TimeCode frame_tc) = 0;
};

class AndCut : public Cut
{
    Cut *left;
    Cut *right;

    bool IsLeftFrameRequired(const int, const TimeCode);
    bool IsRightFrameRequired(const int, const TimeCode);

 public:
    AndCut(Cut *l, Cut *r) : left(l), right(r) {}
    virtual bool IsFrameRequired(const int frame_index, const TimeCode frame_tc);
};


class BeforeTimeCut : public Cut
{
    TimeCode cut_time;
    
 public:
    BeforeTimeCut(const TimeCode tc) : cut_time(tc) {}
    virtual bool IsFrameRequired(const int frame_index, const TimeCode frame_tc);
};

class AfterTimeCut : public Cut
{
    TimeCode cut_time;
    
 public:
    AfterTimeCut(const TimeCode tc) : cut_time(tc) {}
    virtual bool IsFrameRequired(const int frame_index, const TimeCode frame_tc);
};

#endif
