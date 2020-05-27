VPATH =./src
CPP = g++
CXXFLAGS = -std=c++11 -O2 -g 
OBJS = main.o parser.o kv.o network.o coordinator.o logger.o participant.o conf.o ossSocket.o
INCLUDE = -I./src/include  -I./src/third_party/parallel_hashmap  -I./src/third_party/ossSocket
LIB = -lpthread

kvstore2pcsystem : $(OBJS)
	$(CPP) $(CXXFLAGS) -o $@ $(OBJS) $(LIB) $(INCLUDE)
	@rm *.o

main.o : main.cc
	$(CPP) $(CXXFLAGS) -c $^  $(LIB) $(INCLUDE)

parser.o : parser.cc
	$(CPP) $(CXXFLAGS) -c $^  $(LIB) $(INCLUDE)

kv.o : kv.cc
	$(CPP) $(CXXFLAGS) -c $^  $(LIB) $(INCLUDE)

network.o : network.cc
	$(CPP) $(CXXFLAGS) -c $^  $(LIB) $(INCLUDE)

coordinator.o : coordinator.cc
	$(CPP) $(CXXFLAGS) -c $^ $(LIB) $(INCLUDE)

logger.o : logger.cc
	$(CPP) $(CXXFLAGS) -c $^ $(LIB) $(INCLUDE)

participant.o :  participant.cc
	$(CPP) $(CXXFLAGS) -c $^ $(LIB)  $(INCLUDE)

conf.o : conf.cc
	$(CPP) $(CXXFLAGS) -c $^  $(LIB) $(INCLUDE)

ossSocket.o : ./src/third_party/ossSocket/ossSocket.cc
	$(CPP) $(CXXFLAGS) -c $^  $(LIB) $(INCLUDE)

PHONY: clean

clean:
	rm -f ./kvstore2pcsystem ./*.o ./src/*.gch