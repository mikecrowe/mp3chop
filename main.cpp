#define _GNU_SOURCE
#include <getopt.h>
#include "header.h"
#include "mp3_timecode.h"
#include "buffer.h"
#include "file_data_source.h"
#include "mp3_processor.h"

int main(int ac, char *av[])
{
    static struct option long_options[] =
    {
	{ "begin", 1, NULL, 'b' },
	{ "end", 1, NULL, 'e' },
	{ "id3", 0, NULL, 'i' },
	{ 0, 0, 0, 0 }
    };
    
    const char *short_options = "-b:e:i";
    
    MP3Processor processor;
    
    try
    {
	while (optind < ac)
	{
	    int option_index;
	    const int option = getopt_long(ac, av, short_options, long_options, &option_index);
	    
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
}
