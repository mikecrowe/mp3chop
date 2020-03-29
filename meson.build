project('mp3chop', 'cpp', version: '2.0.1')

cc = meson.get_compiler('cpp')
cdata = configuration_data()
check_headers = [
  ['HAVE_FCNTL_H', 'fcntl.h'],
  ['HAVE_UNISTD_H', 'unistd.h'],
]

foreach h : check_headers
  if cc.has_header(h.get(1))
    cdata.set(h.get(0), 1)
  endif
endforeach

check_functions = [
  ['HAVE_GETOPT_LONG', 'getopt_long', '#include<getopt.h>'],
  ['HAVE_GETOPT', 'getopt', '#include<getopt.h>'],
]

foreach f : check_functions
  if cc.has_function(f.get(1), prefix : f.get(2))
    cdata.set(f.get(0), 1)
  endif
endforeach

cdata.set('MP3CHOP_VERSION', meson.project_version())

configure_file(input : 'config.h.meson',
  output : 'config.h',
  configuration : cdata)

sources = [ 'header.cpp',
	    'mp3_timecode.cpp',
	    'buffer.cpp',
	    'file_data_source.cpp',
	    'file_data_sink.cpp',
	    'filter.cpp',
	    'mp3_processor.cpp',
	    'chop.cpp',
	    'xing_frame.cpp' ]

libchop = static_library('chop', sources)

executable('mp3chop', 'main.cpp',
	   link_with: libchop,
	   install: true)

buffer_unittest = executable('buffer_unittest',
			     'buffer_unittest.cpp',
			     link_with: libchop)
test('buffer_unittest', buffer_unittest)

install_man('mp3chop.1')
