#ifndef SOLVE_H
#define SOLVE_H
#include "types.h"

#define MAX_ITER 50
#define EPS 1e-3

int check_convergence(NODE* func, double complex point);
int check_convergence_BM(NODE* func, double complex point1, double complex point2);
void save_points_to_file(const char* filename, NODE* func, double x, double y, double range, int n);

#endif
