#include "filter.h"
#include <stdio.h>

void MultiFilter::Apply(MPEGHeader *header, unsigned char *buffer, unsigned length)
{
    for(Filters::const_iterator i = m_filters.begin(); i != m_filters.end(); ++i)
    {
	(*i)->Apply(header, buffer, length);
    }
}

void SCMSFilter::Apply(MPEGHeader *header, unsigned char *buffer, unsigned length)
{
    if (m_do_original)
    {
	fprintf(stderr, "Setting original to %d\n", m_original);
	header->SetOriginal(m_original);
    }
    if (m_do_copyright)
    {
	fprintf(stderr, "Setting copyright to %d\n", m_copyright);
	header->SetCopyright(m_copyright);
    }
    fprintf(stderr, "original=%d, copyright=%d\n", header->Original(), header->Copyright());
}

