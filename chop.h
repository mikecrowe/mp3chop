#ifndef EDITOR_H
#define EDITOR_H

#include "mp3_timecode.h"

class Chop
{
public:
    virtual bool IsFrameRequired(const int frame_index, const TimeCode frame_tc) = 0;
};

class AndChop : public Chop
{
    Chop *left;
    Chop *right;
	
    bool IsLeftFrameRequired(const int, const TimeCode);
    bool IsRightFrameRequired(const int, const TimeCode);
	
public:
    AndChop(Chop *l, Chop *r) : left(l), right(r) {}
    virtual bool IsFrameRequired(const int frame_index, const TimeCode frame_tc);
};


class BeforeTimeChop : public Chop
{
    TimeCode cut_time;
    
public:
    BeforeTimeChop(const TimeCode tc) : cut_time(tc) {}
    virtual bool IsFrameRequired(const int frame_index, const TimeCode frame_tc);
};

class AfterTimeChop : public Chop
{
    TimeCode cut_time;
    
public:
    AfterTimeChop(const TimeCode tc) : cut_time(tc) {}
    virtual bool IsFrameRequired(const int frame_index, const TimeCode frame_tc);
};

class NullChop : public Chop
{
public:
    virtual bool IsFrameRequired(const int frame_index, const TimeCode frame_tc);
};

#endif
