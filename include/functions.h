#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "types.h"

FUNCTION* create_func_onep(FUNC_ONE_PARAM fun);
FUNCTION* create_func_twop(FUNC_TWO_PARAM fun);
FUNCTION* create_func_const(short number);

TYPE get_func_type(const FUNCTION *func);

double complex evaluate_func_onep(const FUNCTION *func, double complex x);
double complex evaluate_func_twop(const FUNCTION *func, double complex x, double complex y);
double complex evaluate_func_const(const FUNCTION *func);

void destroy_function(FUNCTION *func);

char* print_func_onep(const FUNCTION *func, char* x);
char* print_func_twop(const FUNCTION *func, char* x, char* y);
char* print_func_const(const FUNCTION *func);

#endif

