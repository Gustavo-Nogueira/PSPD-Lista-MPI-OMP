#ifndef UTILS_H_
#define UTILS_H_

#include <mpi.h>

void mpi_token_init(MPI_Win *win, int **mpi_token, int rank);

void mpi_token_free(MPI_Win *win, int **mpi_token, int rank);

void mpi_token_lock(MPI_Win *win, int rank);

void mpi_token_unlock(MPI_Win *win);

void mpi_write_file_offset(char *file_path, int file_offset, char *data, int size);

void write_file_offset(char *file_path, int file_offset, char *data, int size);

#endif /* UTILS_H_ */