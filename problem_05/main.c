#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct CompareElement {
    int value;
    int index;
};

// Custom Reduction
#pragma omp declare reduction(maximum : struct CompareElement : omp_out = omp_in.value > omp_out.value ? omp_in : omp_out)

void selection_sort(int *v, int n) {
    int i, j, min, tmp;

    for (i = 0; i < n - 1; i++) {
        min = i;

        for (j = i + 1; j < n; j++)
            if (v[j] < v[min])
                min = j;

        tmp = v[i];
        v[i] = v[min];
        v[min] = tmp;
    }
}

void omp_selection_sort(int *v, int n) {
    // Sort from back to front
    for (int i = n - 1; i > 0; --i) {
        struct CompareElement emax = {.index = i, .value = v[i]};

        // Reduction to find the maximum value in the [0, j-1] interval (sharing emax attribute)
        #pragma omp parallel for reduction(maximum : emax)
        for (int j = i - 1; j >= 0; --j) {
            if (v[j] > emax.value) {
                emax.value = v[j];
                emax.index = j;
            }
        }

        int tmp = v[i];
        v[i] = emax.value;
        v[emax.index] = tmp;
    }
}

int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int main(int argc, char **argv) {
    char *answers[] = {"wrong", "ok"};
    int *vector_s, *vector_o, *test;
    int i, n, is_ok_s, is_ok_o;
    double elapsed_q, start_q, end_q;
    double elapsed_s, start_s, end_s;
    double elapsed_o, start_o, end_o;

    if (argc < 2) {
        fprintf(stderr, "Uso: %s <número de elementos>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    n = atoi(argv[1]);

    printf("=========================================================\n");
    printf("Problema 05: Ordenacao OMP \n");
    printf("Numero de threads: %d\n", omp_get_max_threads());

    printf("Número de elementos: %d \n", (int)(n));

    printf("=========================================================\n");

    // Allocations
    vector_s = (int *)malloc(n * sizeof(int));
    if (vector_s == NULL) {
        fprintf(stderr, "Out of memory!\n");
        exit(EXIT_FAILURE);
    }

    vector_o = (int *)malloc(n * sizeof(int));
    if (vector_o == NULL) {
        fprintf(stderr, "Out of memory!\n");
        exit(EXIT_FAILURE);
    }

    test = (int *)malloc(n * sizeof(int));
    if (test == NULL) {
        fprintf(stderr, "Out of memory!\n");
        exit(EXIT_FAILURE);
    }


    // Vector random initialization
    srand(time(NULL));
    for (i = 0; i < n; i++) {
        vector_s[i] = rand();
        vector_o[i] = vector_s[i];
        test[i] = vector_s[i];
    }

    // Quicksort
    start_q = omp_get_wtime();
    qsort(test, n, sizeof(int), compare);
    end_q = omp_get_wtime();
    elapsed_q = end_q - start_q;

    // Selection Sort
    start_s = omp_get_wtime();
    selection_sort(vector_s, n);
    end_s = omp_get_wtime();
    elapsed_s = end_s - start_s;

    // Selection Sort (OMP)
    start_o = omp_get_wtime();
    omp_selection_sort(vector_o, n);
    end_o = omp_get_wtime();
    elapsed_o = end_o - start_o; 

    // Check solutions
    is_ok_s = (memcmp(vector_s, test, n * sizeof(int)) == 0);

    is_ok_o = (memcmp(vector_o, test, n * sizeof(int)) == 0);

    // Output
    printf("Resultados: \n");
    
    printf("C Quicksort ...........: %.3f segundos\n", elapsed_q);

    printf("Selection Sort Serial .: %.3f segundos (status=%s)\n", elapsed_s, answers[is_ok_s]);

    printf("Selection Sort OMP ....: %.3f segundos (status=%s)\n", elapsed_o, answers[is_ok_o]);

    printf("=========================================================\n");

    free(vector_s);
    free(vector_o);
    free(test);

    return 0;
}
