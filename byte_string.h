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

/* byte_string.h
 *
 * (C) 2003 empeg ltd, http://www.empeg.com
 * (C) 2004 Mike Crowe
 *
 * This software is licensed under the GNU General Public Licence (see
 * file COPYING).
 *
 */

#ifndef BYTE_STRING_H
#define BYTE_STRING_H

#include <string>
#include <string.h>

//#if defined(__GNUC__)
//#if __GNUC__ > 2

/** Enough of char_traits<UTF16CHAR> that utf16_string compiles and works.
 * Unfortunately doing this is disallowed by the standard (explicit 
 * instantiations of templates in "std" must involve at least one user-defined
 * type) but unfortunately it's also necessary (the standard only requires an
 * implementation to specialise char_traits for char and wchar_t).
 */
template<class T, unsigned int SIZE>
struct custom_char_traits
{
    typedef T char_type;

    static void 
    assign(char_type& __c1, const char_type& __c2)
    { __c1 = __c2; }

    static char_type *assign(char_type* s, size_t n, char_type a)
    {
	for (size_t i=0; i<n; ++i)
	    s[i] = a;
	return s;
    }

    // GCC2 wants that version of assign to be called set for some reason.
    static char_type *set(char_type* s, size_t n, char_type a)
    {
	assign(s, n, a);
    }

    static size_t length(const char_type *s)
    {
	for (size_t i=0; ; ++i)
	    if (!s[i])
		return i;
    }

    static int compare(const char_type *s1, const char_type *s2, size_t n)
    {
	return memcmp(s1, s2, n*SIZE);
    }

    static char_type *copy(char_type *to, const char_type *from, size_t n)
    {
	return (char_type*)memcpy(to, from, n*SIZE);
    }

    static char_type *move(char_type *to, const char_type *from, size_t n)
    {
	return (char_type*)memmove(to, from, n*SIZE);
    }
    
    // http://gcc.gnu.org/ml/libstdc++/2002-08/msg00260.html
    static const char_type* find(const char_type *s, size_t n,
				 const char_type& a)
    { 
	for (const char_type* p = s; size_t(p - s) < n; ++p)
	    if (*p == a) return p;
	return 0;
    }
};

typedef std::basic_string<unsigned char, custom_char_traits<unsigned char, sizeof(unsigned char)> > byte_string;

//#endif /* __GNUC__ > 2 */
//#endif /* defined(__GNUC__) */

#endif // BYTE_STRING_H
