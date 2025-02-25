#include "solve.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>

int check_convergence_MSI(NODE* func, double complex point){
  int i;
  double complex start = evaluate(func, point);
  double complex prev = start;
  for(i = 0; i < MAX_ITER; i++){
    point = prev + evaluate(func, prev) / start;
    if (cabs(point-prev)  < EPS){
      return i;
    } 
    prev = point;
  }
  return MAX_ITER;
}

int check_convergence_BM(NODE* func, double complex point1, double complex point2){
  int hui;
  hui = cabs(evaluate(func, point1) * evaluate(func, point2));
  for (int i = 1; i < 1000; i++){
    if (hui < i){
      return i;
    }
  }
  return 0;
} 

#if 0
void save_points_to_file(const char* filename, NODE* func, double x, double y, double range, int n){
  double step, re, im;
  double complex point;
  int res;
  
  // Размерность массива
  int rows = n;
  int cols = n;
  
  FILE* file = fopen(filename, "w");

  step = (range * 2) / n;

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      re = -range + i * step + x;
      im = range - j * step + y;
      point = re + im * I;
      res = check_convergence(func, point);
      fprintf(file, "%d", res);

      // Добавляем запятую, если это не последний элемент в строке
      if (j < cols - 1) {
        fprintf(file, ",");
      }
    }
    // Переход на новую строку после каждого ряда
    fprintf(file, "\n");
  }
  fclose(file);
  return;
}
#endif

void save_points_to_file(const char* filename, NODE* func, double x, double y, double range, int n) {
    double step = (range * 2) / n;
    int rows = n;
    int cols = n;

    int** results = malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        results[i] = malloc(cols * sizeof(int));
    }

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double re1 = -range + i * step + x;
            double im1 = range - j * step + y;
            double complex point1 = re1 + im1 * I;

            //double re2 = -range + (i + 1) * step + x;
            //double im2 = -range - (j + 1) * step + y;
            //double complex point2 = re2 + im2 * I;
            results[i][j] = check_convergence_MSI(func, point1);
            //results[i][j] = creal(evaluate(func, point1));
        }
    }

    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Ошибка открытия файла");
        return;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%d", results[i][j]);
            if (j < cols - 1) {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
        free(results[i]);
    }
    free(results);
    fclose(file);
}
