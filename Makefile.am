bin_PROGRAMS = mp3chop
mp3chop_SOURCES=\
	header.cpp \
	main.cpp \
	mp3_timecode.cpp \
	buffer.cpp \
	file_data_source.cpp \
	file_data_sink.cpp \
	mp3_processor.cpp \
	chop.cpp \
	xing_frame.cpp

CXXFLAGS+=-DMP3CHOP_VERSION="\"$(VERSION)\""

MAINTAINERCLEANFILES+=\
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

