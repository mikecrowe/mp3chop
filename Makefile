MKDEP=./mkdep
CXX=g++
CXXFLAGS=-c -Wall -pedantic
LD=g++
#LDFLAGS=

SOURCES=header.cpp \
	main.cpp \
	mp3_timecode.cpp \
	buffer.cpp \
	file_data_source.cpp \
	mp3_processor.cpp

OBJS1=$(SOURCES:%.cpp=%.o)
OBJS=$(OBJS1:%.c=%.o)
DEPS=$(OBJS:%.o=%.d)

%.d : %.cpp ; $(MKDEP) $(CXX) $(CXXFLAGS) $< > $@

%.o : %.cpp ; $(CXX) $(CXXFLAGS) -o $@ $<

mp3chop : $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

-include $(DEPS)

clean :
	rm -f *.o mp3chop *\~
