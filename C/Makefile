CXX_FLAGS=-Ofast -march=native -mtune=native

all: jacobi

jacobi: main.cpp
	g++ main.cpp  -o jacobi $(CXX_FLAGS)

asm: main.cpp
	g++ main.cpp  -o jacobi.s $(CXX_FLAGS) -S

debug: main.cpp
	g++ main.cpp -o jacobi -O0  -g -fopenmp

clean:
	rm jacobi
	rm jacobi.s
