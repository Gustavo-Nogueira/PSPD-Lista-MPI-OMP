ARGS = $(filter-out $@,$(MAKECMDGOALS))
get_arg = $(word $1, $(ARGS))

all:
	gcc main.c -fopenmp -o program

run:
	OMP_NUM_THREADS=$(shell expr $(call get_arg, 1)) ./program $(call get_arg, 2)

clean:
	-rm -f ./*.o program

# skip unknown commands (This allows us to accept extra arguments)
%:
	@: