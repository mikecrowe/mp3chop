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

