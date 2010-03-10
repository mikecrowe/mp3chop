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

#ifndef FILTER_H
#define FILTER_H 1

#include <vector>
#include "header.h"

class Filter
{
public:
    virtual void Apply(MPEGHeader *header, unsigned char *buffer, unsigned length) = 0;
};

class MultiFilter : public Filter
{
    typedef std::vector<Filter *> Filters;
    Filters m_filters;

public:
    virtual void Apply(MPEGHeader *header, unsigned char *buffer, unsigned length);
    void Clear()
    {
	m_filters.erase(m_filters.begin(), m_filters.end());
    }
    void AddFilter(Filter *p)
    {
	m_filters.push_back(p);
    }
};

class SCMSFilter : public Filter
{
    bool m_do_original;
    bool m_do_copyright;
    
    bool m_original;
    bool m_copyright;

public:
    SCMSFilter() : m_do_original(false), m_do_copyright(false) {}
    void SetCopyright(bool b)
    {
	m_do_copyright = true;
	m_copyright = b;
    }
    void SetOriginal(bool b)
    {
	m_do_original = true;
	m_original = b;
    }
    virtual void Apply(MPEGHeader *header, unsigned char *buffer, unsigned length);
};

#endif // FILTER_H

