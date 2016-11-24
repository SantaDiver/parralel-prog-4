SRCS = main.c
OBJS = $(SRCS:.cpp=.o)

build:
	mpicxx main.c -o hello -lm -std=c++11
	
run: build
	mpirun -np 1 ./hello matrix.bin vector.bin out.bin

all: run
	
report: build
	mpirun -np 1 ./hello matrix.bin vector.bin out.bin > t1
	echo "" > ./reportdata
	for i in 2 3 4 5 6 7 8 9 10; do \
		mpirun -np $$i ./hello matrix.bin vector.bin out.bin >> ./reportdata < t1; \
	done;
	gnuplot plotrules
	
generate:
	g++ generator.cpp -o generator -lm -std=c++11
	./generator matrix.bin 524 524
	./generator vector.bin 524

	
clean: 
	rm -rf *.o hello