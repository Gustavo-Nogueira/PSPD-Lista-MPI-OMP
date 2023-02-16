#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "fractal.h"

void mpi_token_init(MPI_Win *win, int **mpi_token, int rank) {
    // creation of the "shared" token on process of rank 0
    int win_size = 0;

    if (rank == 0) {
        win_size = sizeof(int);
        MPI_Alloc_mem(win_size, MPI_INFO_NULL, mpi_token);
        *(*mpi_token) = 0;  // initialised to 0
    }

    MPI_Win_create(*mpi_token, win_size, sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, win);
}

void mpi_token_free(MPI_Win *win, int **mpi_token, int rank) {
    // cleaning up
    MPI_Win_free(win);
    if (rank == 0) {
        MPI_Free_mem(*mpi_token);
    }
}

void mpi_token_lock(MPI_Win *win, int rank) {
    int token = -1, inc_token = 1;
    struct timespec sleep_time;
    struct timespec return_time;
    sleep_time.tv_sec = 0;
    sleep_time.tv_nsec = 10 * 1000000;  // 10ms

    while (1) {
        MPI_Win_lock(MPI_LOCK_EXCLUSIVE, 0, 0, *win);
        MPI_Fetch_and_op(&inc_token, &token, MPI_INT, 0, 0, MPI_SUM, *win);
        MPI_Win_unlock(0, *win);

        if (token == 1) return;

        // nanosleep(&sleep_time, &return_time);
    }
}

void mpi_token_unlock(MPI_Win *win) {
    int token = -1, unlock_token = 0;

    MPI_Win_lock(MPI_LOCK_EXCLUSIVE, 0, 0, *win);
    MPI_Fetch_and_op(&unlock_token, &token, MPI_INT, 0, 0, MPI_REPLACE, *win);
    MPI_Win_unlock(0, *win);
}

void mpi_write_file_offset(char *file_path, int file_offset, char *data, int size) {
    MPI_File fp;
    MPI_Status status;
    MPI_Offset offset = file_offset;
    MPI_File_open(MPI_COMM_WORLD, file_path, MPI_MODE_WRONLY, MPI_INFO_NULL, &fp);
    MPI_File_seek(fp, offset, MPI_SEEK_SET);
    MPI_File_write(fp, data, size, MPI_CHAR, &status);
    MPI_File_close(&fp);
}

void write_file_offset(char *file_path, int file_offset, char *data, int size) {
    FILE *fp = fopen(file_path, "r+");
    fseek(fp, 0, SEEK_END);
    fseek(fp, file_offset, SEEK_SET);
    fwrite(data, sizeof(unsigned char), size, fp);
    fclose(fp);
}