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

