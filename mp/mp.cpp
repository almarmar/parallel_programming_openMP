#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <locale.h>

#define N 1000 // размер матрицы

// генерация случайных значений в матрице
void generate_matrix(int** m) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            m[i][j] = rand() % 100; 
        }
    }
}

// выделение памяти под матрицу
int** allocate_matrix() {
    int** m = (int**)malloc(N * sizeof(int*));
    for (int i = 0; i < N; ++i) {
        m[i] = (int*)malloc(N * sizeof(int));
    }
    return m;
}

// освобождение памяти
void free_matrix(int** m) {
    for (int i = 0; i < N; ++i) {
        free(m[i]);
    }
    free(m);
}

// перемножение
void multiply_matrices(int** res, int** m1, int** m2) {
#pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            res[i][j] = 0;
            for (int k = 0; k < N; ++k) {
                res[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    int** m1 = allocate_matrix();
    int** m2 = allocate_matrix();
    int** res = allocate_matrix();

    generate_matrix(m1);
    generate_matrix(m2);

    double start_time = omp_get_wtime(); // засекаем время начала выполнения

    int num_threads = 14; // меняем количество потоков
    omp_set_num_threads(num_threads); 

    // повторяем перемножение матриц до тех пор, пока время выполнения не достигнет 2 секунд
    do {
        multiply_matrices(res, m1, m2);
    } while (omp_get_wtime() - start_time < 2.0);

    double end_time = omp_get_wtime(); // засекаем время окончания выполнения
    double elapsed_time = end_time - start_time;

    printf("время: %f сек.\n", elapsed_time);

    free_matrix(m1);
    free_matrix(m2);
    free_matrix(res);

    return 0;
}

