#ifndef EDITOR_H
#define EDITOR_H

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
};


class BeforeTimeCut : public Cut
{
    TimeCode cut_time;
    
 public:
    CutBeforeTimeEditor(const TimeCode tc) : Editor(n), cut_time(tc) {}
    virtual bool IsFrameRequired(const int frame_index, const TimeCode frame_tc);
};

class AfterTimeCut : public Cut
{
    TimeCode cut_time;
    
 protected:
    virtual bool InternalIsRequired(const int frame_index, const TimeCode frame_tc);
    
 public:
    CutAfterTimeEditor(const TimeCode tc) : Editor(n), cut_time(tc) {}
};

#endif
