#include "platform_config.h"
#include <iostream>
#include "header.h"
#include "mp3_timecode.h"
#include "buffer.h"
#include "file_data_source.h"
#include "mp3_processor.h"
#include <stdio.h>

// If we have GNU getopt_long then make sure we include its declaration.
#if defined(HAVE_GETOPT_LONG)
#if !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#endif
#include <getopt.h>
#endif

#if defined(HAVE_GETOPT)
#include <getopt.h>
#endif

void Help()
{
    fprintf(stderr,
	    "mp3chop: A program for chopping out bits of mp3 files.\n"
	    "(C) 1999-2003 Mike Crowe, <mac@fysh.org>\n"
	    "Version " VERSION "\n"
	    "\n"
	    "mp3chop is distributed with ABSOLUTELY NO WARRANTY under the terms\n"
	    "of the GNU General Public License Version 2.\n"
	    "\n"
	    "Usage: mp3chop [options] < infile > outfile\n"
	    "\n"
	    "   --strip-id3=1, -s 1        String ID3 version 1 tags\n"
	    "   --strip-id3=2, -s 2        String ID3 version 2 tags\n"
	    "   --begin=<time>, -b <time>  Do not output anything before <time>\n"
	    "   --end=<time>, -e <time>    Do not output anything after <time>\n"
	    "   --help, -h                 Display this help\n"
	    "   --dump-header, -d          Dump the header of the first valid frame\n"
	    "   --dump-all -D              Dump the headers of every frame\n"
	    "   --copyright=0|1, -c 0|1    Force copyright flag\n"
	    "   --original=0|1, -o 0|1     Force original flag\n"
	    "\n"
	    "   <time> is of the form 'mm:ss:hh' mm=minutes, ss=seconds, hh=hundredths\n"
	    "\n");
}

int main(int ac, char *av[])
{
#ifdef HAVE_GETOPT_LONG
    static struct option long_options[] =
    {
	{ "begin", 1, NULL, 'b' },
	{ "end", 1, NULL, 'e' },
	{ "copyright", 1, NULL, 'c' },
	{ "original", 1, NULL, 'o' },
	{ "strip-id3", 1, NULL, 's' },
	{ "help", 0, NULL, 'h' },
	{ "dump-header", 0, NULL, 'd' },
	{ "dump-all", 0, NULL, 'D' },
	{ 0, 0, 0, 0 }
    };
#endif // HAVE_GETOPT_LONG

#ifdef HAVE_GETOPT
    const char *short_options = "-b:e:c:o:s:hdv";
#endif // HAVE_GETOPT
    
    MP3Processor processor;
    processor.SetKeepID3V1(true);
    processor.SetKeepID3V2(true);
    
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
	    case 'c':
		processor.HandleForceCopyright(atoi(optarg));
		break;
	    case 'o':
		processor.HandleForceOriginal(atoi(optarg));
		break;
	    case 's':
		if (strcmp(optarg, "1") == 0)
		{
		    processor.SetKeepID3V1(false);
		}
		else if (strcmp(optarg, "2") == 0)
		{
		    processor.SetKeepID3V2(false);
		}
		else
		{
		    std::cerr << "Unknown strip specification." << std::endl;
		}
		break;
	    case '1':
	    case 'h':
		Help();
		return 0;

	    case 'd':
		processor.HandleMode('d');
		break;

	    case 'D':
		processor.HandleMode('D');
		break;
		
	    case '?':
		return 1;
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
