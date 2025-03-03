#ifndef TYPES_H
#define TYPES_H

#include <complex.h>

#define MAX_STRING 2048
//Структура для дерева
typedef struct Node NODE;

// Определение перечислений (enum)
typedef enum {
  X = 0,
  SQRT = 1,
  LN = 2,
  SIN = 3,
  COS = 4,
  TAN = 5,
  CTAN = 6,
  EXP = 7,
  CONJ = 8,
  END_ONE_P = 9
} FUNC_ONE_PARAM;

typedef enum {
  PLUS = 0,
  MINUS = 1, 
  MULT = 2,
  FRAC = 3,
  POW = 4,
  END_TWO_P = 5
} FUNC_TWO_PARAM;

typedef enum {
  ONE_P, // Функция с одним параметром
  TWO_P,  // Функция с двумя параметрами
  CONST  // константа
} TYPE;

// Определение структуры function
typedef struct function FUNCTION;

#endif
