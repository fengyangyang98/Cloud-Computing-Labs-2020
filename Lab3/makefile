VPATH =./src
CPP = g++
CXXFLAGS = -std=c++11 -O2 -g 
OBJS = main.o read_conf.o
LIBS = 

kvstore2pcsystem : $(OBJS)
	$(CPP) $(CXXFLAGS) -o $@ $(OBJS) $(LIBS)
	@rm *.o ./src/*.gch 

main.o : main.cpp
	$(CPP) $(CXXFLAGS) -c $^ $(LIBS)

read_conf.o : read_conf.hpp read_conf.cpp
	$(CPP) $(CXXFLAGS) -c $^ $(LIBS)

PHONY: clean

clean:
	rm -f ./kvstore2pcsystem ./*.o ./src/*.gch