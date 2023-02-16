ARGS = $(filter-out $@,$(MAKECMDGOALS))
get_arg = $(word $1, $(ARGS))

all:
	gcc -c main.c -fopenmp
	gcc -c fractal.c 
	gcc main.o fractal.o -fopenmp -lm -o program

run:
	OMP_NUM_THREADS=$(shell expr $(call get_arg, 1)) ./program $(call get_arg, 2)

clean:
	-rm -f ./*.o ./*.bmp program

# skip unknown commands (This allows us to accept extra arguments)
%:
	@: