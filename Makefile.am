#
# mp3chop
#
# (C) 2000-2010 Mike Crowe <mac@mcrowe.com>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
# USA

ACLOCAL_AMFLAGS = -I autotools

bin_PROGRAMS=mp3chop
mp3chop_SOURCES=\
	header.cpp \
	main.cpp \
	mp3_timecode.cpp \
	buffer.cpp \
	file_data_source.cpp \
	file_data_sink.cpp \
	filter.cpp \
	mp3_processor.cpp \
	chop.cpp \
	xing_frame.cpp \
	buffer.h \
	byte_string.h \
	chop.h \
	config.h \
	file_data_sink.h \
	file_data_source.h \
	file_exception.h \
	filter.h \
	header.h \
	mp3_processor.h \
	mp3_timecode.h \
	platform_config.h \
	types.h \
	trace.h \
	xing_frame.h

man_MANS=mp3chop.1

# We don't want to distribute types2.h
CONFIG_CLEAN_FILES=\
	$(srcdir)/types2.h

MAINTAINERCLEANFILES=\
	$(srcdir)/Makefile.in \
	$(srcdir)/configure \
	$(srcdir)/config.sub \
	$(srcdir)/config.guess \
	$(srcdir)/install-sh \
	$(srcdir)/config.h.in \
	$(srcdir)/stamp-h.in \
	$(srcdir)/mkinstalldirs \
	$(srcdir)/missing \
	$(srcdir)/aclocal.m4

EXTRA_DIST=$(man_MANS)
