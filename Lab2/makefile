VPATH =./src
CPP = g++
CXXFLAGS = -std=c++11 -O2 -g
OBJS = main.o util.o
LIBS = -lpthread

httpserver.out : $(OBJS)
	$(CPP) $(CXXFLAGS) -o $@ $(OBJS) $(LIBS)
	rm *.o ./src/*.h.gch

main.o : main.cpp
	$(CPP) $(CXXFLAGS) -c $^ $(LIBS)

util.o : util.h util.cpp
	$(CPP) $(CXXFLAGS) -c $^ $(LIBS)

PHONY: clean

clean:
	rm httpserver.out *.o ./src/*.h.gch