CPP=g++
CPPFLAGS=-Wall

ANSISTD=-ansi
ISOSTD=-std=c++11

LIBS=-lpthread

SRCMODULES=ThreadList.cpp
OBJMODULES=$(SRCMODULES:.cpp=.o)
HPPMODULES=$(SRCMODULES:.cpp=.hpp)

%.o: %.cpp %.hpp
	$(CPP) $(CPPFLAGS) -g $< -c -o $(LIBS) $@ $(ISOSTD)

main: main.cpp $(OBJMODULES)
	$(CPP) $(CPPFLAGS) -s $^ -o $@ $(LIBS) $(ISOSTD)