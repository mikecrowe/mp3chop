#include "config.h"
#include "header.h"
#include "mp3_timecode.h"
#include "buffer.h"
#include "file_data_source.h"
#include "mp3_processor.h"
#include <stdio.h>

// If we have GNU getopt_long then make sure we include its declaration.
#if defined(HAVE_GETOPT_LONG)
#define _GNU_SOURCE
#include <getopt.h>
#endif

#if defined(HAVE_GETOPT)
#include <getopt.h>
#endif

void Help()
{
    fprintf(stderr,
	    "mp3chop: A program for chopping out bits of mp3 files.\n"
	    "(C) 1999-2000 Mike Crowe, <mac@fysh.org>\n"
	    "Version " MP3CHOP_VERSION "\n"
	    "\n"
	    "mp3chop is distributed with ABSOLUTELY NO WARRANTY under the terms\n"
	    "of the GNU General Public License Version 2.\n"
	    "\n"
	    "Usage: mp3chop [options] < infile > outfile\n"
	    "\n"
	    "   --id3, -i                  Preserve id3 version 1 tags\n"
	    "   --begin=<time>, -b <time>  Do not output anything before <time>\n"
	    "   --end=<time>, -e <time>    Do not output anything after <time>\n"
	    "   --help, -h                 Display this help\n"
	    "\n"
	    "   <time> is of the form 'mm:ss:hh' mm=minutes, ss=seconds, hh=hundredths\n"
	    "\n");
}

int main(int ac, char *av[])
{
    static struct option long_options[] =
    {
	{ "begin", 1, NULL, 'b' },
	{ "end", 1, NULL, 'e' },
	{ "id3", 0, NULL, 'i' },
	{ "help", 0, NULL, 'h' },
	{ 0, 0, 0, 0 }
    };
    
    const char *short_options = "-b:e:ih";
    
    MP3Processor processor;
    
    try
    {
	while (optind < ac)
	{
	    int option_index;
#ifdef HAVE_GETOPT_LONG
	    const int option = getopt_long(ac, av, short_options, long_options, &option_index);
#else
#ifdef HAVE_GETOPT
	    const int option = getopt(ac, av, short_options);
#else
#error Need getopt or getopt_long to work
#endif
#endif
	    
	    switch (option)
	    {
	    case 1:
		processor.HandleFile(optarg);
		break;
	    case 'b':
		processor.HandleBeginTimeCode(optarg);
		break;
	    case 'e':
		processor.HandleEndTimeCode(optarg);
		break;
	    case 'i':
		processor.SetKeepID3(true);
		break;
	    case 'h':
		Help();
		exit(0);
	    case '?':
		exit(1);
	    }
	}
	processor.HandleEnd();
    }
    catch (MP3Processor::Exception &e)
    {
	e.Report();
    }
    return 0;
}
