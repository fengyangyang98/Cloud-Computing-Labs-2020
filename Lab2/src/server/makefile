VPATH =../ ./
CPP = g++
CXXFLAGS = -std=c++11 -O2 -g 
OBJS = server.o util.o transport.o
LIBS = -lpthread

server : $(OBJS)
	$(CPP) $(CXXFLAGS) -o $@ $(OBJS) $(LIBS)
	rm *.o ../*.gch

server.o : server.cpp
	$(CPP) $(CXXFLAGS) -c $^ $(LIBS)

util.o : util.hpp util.cpp
	$(CPP) $(CXXFLAGS) -c $^ $(LIBS)

transport.o : transport.hpp transport.cpp
	$(CPP) $(CXXFLAGS) -c $^ $(LIBS)

PHONY: clean

clean:
	rm httpserver.out *.o ./src/*.h.gch