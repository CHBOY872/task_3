CPP=g++
CPPFLAGS=-Wall

ANSISTD=-ansi
ISOSTD=-std=c++11

LIBS=-lpthread

DEFINES=MAXTHREADS=4

SRCMODULES=$(shell find . -name '*.cpp' | grep -v 'main.cpp')
OBJMODULES=$(SRCMODULES:.cpp=.o)
HPPMODULES=$(SRCMODULES:.cpp=.hpp)

%.o: %.cpp %.hpp
	$(CPP) $(CPPFLAGS) -g $< -c -o $@ $(ISOSTD)

main: main.cpp $(OBJMODULES)
	$(CPP) $(CPPFLAGS) -g -D$(DEFINES) $^ $(LIBS) -o $@ $(ISOSTD)

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif

deps.mk: $(SRCMODULES)
	$(CC) -MM $^ > $@

clean:
	find . -name '*.o' -delete

all:
	make main
	make clean
