/*
 * mp3chop
 *
 * (C) 2000-2010 Mike Crowe <mac@mcrowe.com>
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
