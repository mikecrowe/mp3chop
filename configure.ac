dnl
dnl mp3chop
dnl
dnl (C) 2000-2010 Mike Crowe <mac@mcrowe.com>
dnl
dnl This program is free software; you can redistribute it and/or
dnl modify it under the terms of the GNU General Public License as
dnl published by the Free Software Foundation; either version 2 of
dnl the License, or (at your option) any later version.
dnl
dnl This program is distributed in the hope that it will be useful, but
dnl WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program; if not, write to the Free Software
dnl Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
dnl USA


AC_INIT([mp3chop],[1.08],[mac@fysh.org])
AM_INIT_AUTOMAKE([-Wall -Werror])
AC_CONFIG_MACRO_DIR(autotools)
AC_CONFIG_SRCDIR(chop.cpp)

m4_include([autotools/ax_create_stdint_h.m4])

AC_PROG_CXX
AC_LANG(C++)
AC_HEADER_STDC
AX_CREATE_STDINT_H

AC_TYPE_SIZE_T

AC_CHECK_FUNCS(getopt_long getopt)
AC_CHECK_FUNCS(strerror)
AC_CHECK_FUNCS(memmove)
AC_FUNC_MEMCMP(memcmp)

AC_CHECK_HEADERS(fcntl.h)
AC_CHECK_HEADERS(string.h)
AC_CHECK_HEADERS(unistd.h)
AC_CHECK_HEADERS(fcntl.h)
AC_CHECK_HEADERS(assert.h)

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

