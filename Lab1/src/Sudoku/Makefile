OBJS = sudoku_solver.o main.o
CPP = g++
FLAGES = -O2 

sudoku: $(OBJS)
	$(CPP) -o $@ $(OBJS)

main.o: main.cpp main.h
	$(CPP) $(FLAGES) -c $^

sudoku_solver.o: sudoku_solver.cpp sudoku_solver.h
	$(CPP) $(FLAGES) -c $^

PHONY: clean

clean:
	rm *.o *.h.gch sudoku
	clear