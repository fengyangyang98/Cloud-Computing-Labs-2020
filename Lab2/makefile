VPATH =./src
CPP = g++
CXXFLAGS = -std=c++11 -O2 -g 
OBJS = main.o util.o transport.o
LIBS = -lpthread

httpserver : $(OBJS)
	$(CPP) $(CXXFLAGS) -o $@ $(OBJS) $(LIBS)
	rm *.o ./src/*.gch 

main.o : main.cpp threadpool.hpp
	$(CPP) $(CXXFLAGS) -c $^ $(LIBS)

util.o : util.hpp util.cpp
	$(CPP) $(CXXFLAGS) -c $^ $(LIBS)

transport.o : transport.hpp transport.cpp
	$(CPP) $(CXXFLAGS) -c $^ $(LIBS)

PHONY: clean

clean:
	rm httpserver.out *.o ./src/*.h.gch