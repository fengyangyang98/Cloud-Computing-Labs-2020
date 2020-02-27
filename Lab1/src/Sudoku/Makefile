OBJS = sudoku_solver.o main.o io.o thread_solver.o
CPP = g++
FLAGES = -std=c++11 -O2 

sudoku: $(OBJS)
	$(CPP) -o $@ $(OBJS) -lpthread

main.o: main.cpp main.h
	$(CPP) $(FLAGES) -c $^ -lpthread

sudoku_solver.o: sudoku_solver.cpp sudoku_solver.h
	$(CPP) $(FLAGES) -c $^ -lpthread

io.o: io.cpp io.h
	$(CPP) $(FLAGES) -c $^ -lpthread

thread_solver.o: thread_solver.cpp thread_solver.h
	$(CPP) $(FLAGES) -c $^ -lpthread

PHONY: clean

clean:
	rm *.o *.h.gch sudoku
	clear