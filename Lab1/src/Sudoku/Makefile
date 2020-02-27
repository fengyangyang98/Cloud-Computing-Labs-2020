OBJS = sudoku_solver.o main.o
CPP = g++
FLAGES = -O2 

sudoku: $(OBJS)
	$(CPP) -o $@ $(OBJS)

main.o: main.cpp
	$(CPP) $(FLAGES) -c $^

sudoku_solver.o: sudoku_solver.cpp 
	$(CPP) $(FLAGES) -c $^

PHONY: clean

clean:
	rm *.o sudoku
	clear