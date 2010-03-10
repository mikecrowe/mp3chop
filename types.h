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

#ifndef TYPES_H
#define TYPES_H 1

// Get hold of stdint.h if we have it, otherwise fake it up.
#include "types2.h"

typedef uint8_t BYTE;
typedef uint64_t UINT64;

#ifdef _MSC_VER
#define UINT64LITERAL(LLL) LLL ## i64
#else
#define UINT64LITERAL(LLL) LLL ## LL
#endif

typedef int ERROR;

const int EOUTOFDATA = 21075;

#define SUCCEEDED(x) ((x) >= 0)
#define FAILED(x) ((x) < 0)

#endif
