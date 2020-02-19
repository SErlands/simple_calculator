OBJECTS = simple_calculator.o

calculator: $(OBJECTS) makefile
	g++ -std=c++17 -Wpedantic -Wall -Wextra -Weffc++ -g -o simple_calculator $(OBJECTS)

simple_calculator.o: simple_calculator.cc
	g++ -std=c++17 -Wpedantic -Wall -Wextra -Weffc++ -c simple_calculator.cc

clean:
	@ rm -irf *.o *.gch core

