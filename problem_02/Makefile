ARGS = $(filter-out $@,$(MAKECMDGOALS))
get_arg = $(word $1, $(ARGS))

all:
	mpicc -c main.c 
	gcc -c fractal.c 
	mpicc -c utils.c
	mpicc fractal.o utils.o main.o -o program -lm

run: 
	mpirun --hostfile $(call get_arg, 1) -np $(call get_arg, 2) ./program $(call get_arg, 3)

clean:
	-rm -f ./*.o ./*.bmp program

# skip unknown commands (This allows us to accept extra arguments)
%:
	@: