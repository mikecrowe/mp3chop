bin_PROGRAMS:=mp3chop
mp3chop_SOURCES:=\
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

man_MANS:=mp3chop.1

# We don't want to distribute types2.h
CONFIG_CLEAN_FILES:=\
	$(srcdir)/types2.h

MAINTAINERCLEANFILES:=\
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
