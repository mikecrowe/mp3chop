/*
 * mp3chop
 *
 * (C) 2000-2020 Mike Crowe <mac@mcrowe.com>
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

#ifndef FILE_EXCEPTION_H
#define FILE_EXCEPTION_H 1

#include <string>
#include <errno.h>
#include <string.h>
#include "config.h"

class FileException
{
    int error_number;

public:
    FileException() : error_number(errno) {}
    FileException(int e) : error_number(e) {}

    const std::string Description() { return strerror(error_number); }
    int Number() { return error_number; }
};

#endif
