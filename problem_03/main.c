#include <fcntl.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "fractal.h"

#define HEADER_OFFSET 54
#define OUTFILE "out_julia_normal.bmp"

int exec_hist[100] = {0};

int main(int argc, char *argv[]) {
    int h_fractal = atoi(argv[1]);              // height
    int w_fractal = 2 * h_fractal;              // width
    int a_fractal = h_fractal * w_fractal * 3;  // area
    int r_area_fractal = 3 * w_fractal;         // row area

    printf("=========================================================\n");
    printf("Problema 03: Fractal OMP - Escrita Paralela \n");
    printf("Numero de threads: %d\n", omp_get_max_threads());

    printf("Area do fractal: %d \n", a_fractal);
    printf("Dimensoes do fractal: %d x %d\n", w_fractal, h_fractal);

    printf("=========================================================\n");
    printf("Gerando Fractal...\n");

    // Write bmp file header
    FILE *output_file_fp = fopen(OUTFILE, "w");
    write_bmp_header(output_file_fp, w_fractal, h_fractal);
    fclose(output_file_fp);

    // Posix open bmp file
    int output_file_fd = open(OUTFILE, O_RDWR, 0644);

    // Calculate and write fractal
#pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < h_fractal; i++) {
        unsigned char *pixel_array = calloc(r_area_fractal, sizeof(unsigned char));
        int offset = i * r_area_fractal + HEADER_OFFSET;

        calculate_julia_fractal(pixel_array, h_fractal, i, i);

        pwrite(output_file_fd, pixel_array, r_area_fractal, offset);  // offset-based parallel writing

        exec_hist[omp_get_thread_num()]++;

        free(pixel_array);
    }

    for (int i = 0; i < omp_get_max_threads(); i++) {
        printf("Total de Linhas do Fractal Criadas pela Thread %d: %d.\n", i, exec_hist[i]);
    }

    close(output_file_fd);

    printf("=========================================================\n");

    return 0;
}