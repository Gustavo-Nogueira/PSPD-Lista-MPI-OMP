#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "fractal.h"
#include "utils.h"

#define HEADER_OFFSET 54
#define OUTFILE "out_julia_normal.bmp"

int main(int argc, char *argv[]) {
    FILE *output_file;
    int process_rank, numof_process;

    // Inicializacao do mpi
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numof_process);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    if ((argc <= 1) | (atoi(argv[1]) < 1)) {
        fprintf(stderr, "Entre 'N' como um inteiro positivo! \n");
        return -1;
    }

    // Leitura dos parametros de entrada
    int h_fractal = atoi(argv[1]);              // height
    int w_fractal = 2 * h_fractal;              // width
    int a_fractal = h_fractal * w_fractal * 3;  // area

    if (process_rank == 0) {
        printf("=========================================================\n");
        printf("Problema 02: Fractal MPI - Escrita Paralela \n");
        printf("Numero de processos: %d\n", numof_process);

        printf("Area do fractal: %d \n", a_fractal);
        printf("Dimensoes do fractal: %d x %d\n", w_fractal, h_fractal);

        // Escreve o cabeçalho do arquivo
        output_file = fopen(OUTFILE, "w");
        write_bmp_header(output_file, w_fractal, h_fractal);
        fclose(output_file);
    }

    if (process_rank == 0) {
        printf("=========================================================\n");
        printf("Gerando Fractal...\n");
    }

    int addt = numof_process == process_rank + 1 ? h_fractal % numof_process : 0;
    int chunk = h_fractal / numof_process;
    int start = chunk * process_rank;
    int end = start + chunk + addt - 1;
    int tot_bytes = (end - start + 1) * w_fractal * 3;
    int offset = start * w_fractal * 3 + HEADER_OFFSET;
    unsigned char *pixel_array = calloc(tot_bytes + 10, sizeof(unsigned char));

    calculate_julia_fractal(pixel_array, h_fractal, start, end);

    printf("[Rank %d] Inicio Escrita no Intervalo: %d-%d \n", process_rank, start, end);

    mpi_write_file_offset(OUTFILE, offset, pixel_array, tot_bytes);

    printf("[Rank %d] Fim Escrita no Intervalo: %d-%d \n", process_rank, start, end);

    // Free pointers
    free(pixel_array);

    if (process_rank == 0) {
        printf("=========================================================\n");
    }

    MPI_Finalize();

    return 0;
} /* fim-programa */
