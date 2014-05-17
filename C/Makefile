CXX_FLAGS=-Ofast -march=native -mtune=native -std=c++11

all: jacobi

jacobi: main.cpp
	g++ main.cpp  -o jacobi $(CXX_FLAGS)

profile: main.cpp
	g++ main.cpp -fprofile-generate=prof.out -o jacobi $(CXX_FLAGS)
	./jacobi
	g++ main.cpp -fprofile-use=prof.out -o jacobi $(CXX_FLAGS)

profile-asm: main.cpp
	g++ main.cpp -fprofile-generate=prof.out -o jacobi $(CXX_FLAGS)
	./jacobi
	g++ main.cpp -fprofile-use=prof.out -o jacobi.s -S $(CXX_FLAGS)

asm: main.cpp
	g++ main.cpp  -o jacobi.s $(CXX_FLAGS) -S

debug: main.cpp
	g++ main.cpp -o jacobi -O0  -g -fopenmp

clean:
	rm jacobi
	rm jacobi.s
