project('mp3chop', 'cpp', version: '2.0.1')

cc = meson.get_compiler('cpp')
cdata = configuration_data()
check_headers = [
  ['HAVE_ASSERT_H', 'assert.h'],
  ['HAVE_FCNTL_H', 'fcntl.h'],
  ['HAVE_INTTYPES_H', 'inttypes.h'],
  ['HAVE_MEMORY_H', 'memory.h'],
  ['HAVE_STDINT_H', 'stdint.h'],
  ['HAVE_STDLIB_H', 'stdlib.h'],
  ['HAVE_STRINGS_H', 'strings.h'],
  ['HAVE_STRING_H', 'string.h'],
  ['HAVE_SYS_STAT_H', 'sys/stat.h'],
  ['HAVE_SYS_TYPES_H', 'sys/types.h'],
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
  ['HAVE_STRERROR', 'strerror', '#include<string.h>'],
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
	    'main.cpp',
	    'mp3_timecode.cpp',
	    'buffer.cpp',
	    'file_data_source.cpp',
	    'file_data_sink.cpp',
	    'filter.cpp',
	    'mp3_processor.cpp',
	    'chop.cpp',
	    'xing_frame.cpp' ]

executable('mp3chop', sources, install: true)

buffer_unittest_sources = [ 'buffer_unittest.cpp',
			    'buffer.cpp',
			    'file_data_source.cpp' ]
buffer_unittest = executable('buffer_unittest', buffer_unittest_sources)
test('buffer_unittest', buffer_unittest)

install_man('mp3chop.1')
