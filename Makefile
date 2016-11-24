SRCS = main.c
OBJS = $(SRCS:.cpp=.o)

build:
	mpicxx main.c -o hello -lm -std=c++11
	
run: build
	mpirun -np 4 ./hello matrix.bin vector.bin out.bin

all: run
	
report: build
	echo "Primes: " > out.txt
	echo "" > ./reportdata
	for i in 1 2 3 4 5 6 7 8 9 10; do \
		mpirun -np $$i ./hello 1 700000 out.txt >> ./reportdata; \
	done;
	gnuplot plotrules
	
generate:
	g++ generator.cpp -o generator -lm -std=c++11
	./generator matrix.bin 100 100
	./generator vector.bin 100

	
clean: 
	rm -rf *.o hello