GCC_FLAGS=-std=c++11 -O3

make: kosaraju.o main.cpp
	g++ $(GCC_FLAGS) kosaraju.o main.cpp -o main.out

kosaraju.o: ../util/kosaraju.cpp ../util/kosaraju.h
	g++ $(GCC_FLAGS) -c ../util/kosaraju.cpp

characteristics: characteristics.cpp
	g++ $(GCC_FLAGS) characteristics.cpp -o characteristics.out