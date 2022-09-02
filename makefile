CPP=g++
CPPFLAGS=-Wall

ANSISTD=-ansi
ISOSTD=-std=c++11

LIBS=-lpthread

DEFINES=MAXTHREADS=4

SRCMODULES=ThreadList.cpp List.cpp DirectHandle.cpp ThreadFuncs.cpp
OBJMODULES=$(SRCMODULES:.cpp=.o)
HPPMODULES=$(SRCMODULES:.cpp=.hpp)

%.o: %.cpp %.hpp
	$(CPP) $(CPPFLAGS) -g $< -c -o $@ $(ISOSTD)

main: main.cpp $(OBJMODULES)
	$(CPP) $(CPPFLAGS) -g -D$(DEFINES) $^ -o $@ $(ISOSTD)