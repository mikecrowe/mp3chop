#

mp3chop : header.o main.o mp3_timecode.o buffer.o
	g++ -o mp3chop $^

header.o : header.cpp header.h
	g++ -c header.cpp

main.o : main.cpp header.h mp3_timecode.h buffer.h
	g++ -c main.cpp

buffer.o : buffer.h buffer.cpp
	g++ -c buffer.cpp

mp3_timecode.o : mp3_timecode.cpp mp3_timecode.h
	g++ -c mp3_timecode.cpp

clean :
	rm -f *.o mp3chop *\~
