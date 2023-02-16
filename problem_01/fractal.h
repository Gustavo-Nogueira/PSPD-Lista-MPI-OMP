#ifndef FRACTAL_H_
#define FRACTAL_H_

#include <stdio.h>

int compute_julia_pixel(int x, int y, int largura, int altura, float tint_bias, unsigned char *rgb);

int write_bmp_header(FILE *f, int largura, int altura);

void calculate_julia_fractal(char *pixel_array, int h_fractal, int start_row, int end_row);

#endif /* FRACTAL_H_ */