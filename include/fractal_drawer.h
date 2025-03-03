#ifndef FRACTAL_DRAWER_H
#define FRACTAL_DRAWER_H
#include "types.h"

#define WIDTH 200
#define HEIGHT 200
#define RESOLUTION 100

typedef struct FracParams FracParams;


int init();
void run_fractal_loop(NODE* func);
#endif
