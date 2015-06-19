#netlib static library makefile

CFLAGS += -Wall

#include file directorys
CPPFLAGS += -I../../include
CPPFLAGS += -I../../include/network

#external library directorys and files

SRCS=netlib.cpp netlibimp.cpp

OBJECTS=$(patsubst %.cpp,%.o,$(SRCS))
DEPENDENCY=$(patsubst %.cpp,%.d,$(SRCS))
OUTPUTFILE=../../lib/linux/network.a

.PHONY : all clean
#generate dependency files

all : $(OUTPUTFILE)

$(DEPENDENCY) : $(SRCS)
	gcc $(CFLAGS) $(CPPFLAGS) -MM $< > $@

include $(SRCS:.cpp=.d)

$(OUTPUTFILE) : $(OBJECTS)
	ar rcs $(OUTPUTFILE) $(OBJECTS) 

clean : 
	rm -f *.d *.o $(OUTPUTFILE)
