#include "functions.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

struct function{
  TYPE type;
  union {
    FUNC_ONE_PARAM one_p;
    FUNC_TWO_PARAM two_p;
    double complex num;
  };
};

FUNCTION *create_func_onep(FUNC_ONE_PARAM fun){
  FUNCTION *res = calloc(1, sizeof(FUNCTION));
  res->type = ONE_P;
  res->one_p = fun;
  return res;
}

FUNCTION *create_func_twop(FUNC_TWO_PARAM fun){
  FUNCTION *res = calloc(1, sizeof(FUNCTION));
  res->type = TWO_P;
  res->two_p = fun;
  return res;
}

FUNCTION *create_func_const(double complex number){
  FUNCTION *res = calloc(1, sizeof(FUNCTION));
  res->type = CONST;
  res->num = number;
  return res;
}

TYPE get_func_type(const FUNCTION *func){
  return func->type;
}

double complex evaluate_func_onep(const FUNCTION *func, double complex x){
  assert(func->type == ONE_P);
  double complex res;
  switch(func->one_p){
    case X:
      res = x; break;
    case SQRT:
      res = csqrt(x); break;
    case LN:
      res = clog(x); break;
    case SIN:
      res = csin(x); break;
    case COS:
      res = ccos(x); break;
    case TAN:
      res = ctan(x); break;
    case CTAN:
      res = 1/ctan(x); break; // TODO: деление на 0
    case EXP:
      res = cexp(x); break;
    case CONJ:
      res = conj(x); break;
    default:
      return 0;
  }
  return res;
}

double complex evaluate_func_twop(const FUNCTION *func, double complex x, double complex y){ 
  assert(func->type == TWO_P);
  double complex res;
  switch(func->two_p){
    case PLUS:
      res = x + y; break;
    case MINUS:
      res = x - y; break;
    case MULT:
      res = x * y; break;
    case FRAC:
      res = x / y; break; //TODO: деление на 0
    case POW:
      res = cpow(x, y); break;
    default:
      return 0;
  }
  return res;
}

double complex evaluate_func_const(const FUNCTION *func){
  assert(func->type == CONST);
  return (double complex)func->num;
}

void destroy_function(FUNCTION *func){
  free(func);
}

// Функция для безопасного выделения памяти и создания строки вида "fun_name(arg)"
char* make_onep(const char* fun_name, char* arg, int is_brackets) {
  char *buf = malloc(MAX_STRING);
  if (!buf) return NULL;
  buf[0]='\0';
  
  strcpy(buf, fun_name);
  strcat(buf, "{");

  if (is_brackets)
    strcat(buf, "\\left(");

  strcat(buf, arg);

  if (is_brackets)
    strcat(buf, "\\right)");

  strcat(buf, "}");
  return buf;
}

char* make_twop(char* x, const char* fun, char* y){
  char *buf = malloc(MAX_STRING);
  buf[0]='\0';
  
  strcat(buf, "\\left(");
  strcat(buf, x);
  strcat(buf, fun);
  strcat(buf, y);
  strcat(buf, "\\right)");

  
  return buf; 
}

// Функция для вывода функций с одним аргументом
char* print_func_onep(const FUNCTION *func, char* x) {
  assert(func->type == ONE_P);

  switch(func->one_p) {
    case X:
      return strdup(x); // strdup зачем?
    case SQRT:
      return make_onep("\\sqrt", x, 0);
    case LN:
      return make_onep("\\ln", x, 1);
    case SIN: 
      return make_onep("\\sin", x, 1);
    case COS: 
      return make_onep("\\cos", x, 1);
    case TAN:
      return make_onep("\\tan", x, 1);
    case CTAN:
      return make_onep("\\cot", x, 1);
    case EXP: 
      return make_onep("e^", x, 1);
    case CONJ:
      return make_onep("\\con", x, 0);
    default: 
      return strdup(x);
    }
}



// Функция для вывода функций с двумя аргументами
char* print_func_twop(const FUNCTION *func, char* x, char* y) {
  assert(func->type == TWO_P);

  char *buf;
  
  switch(func->two_p) {
    case PLUS: 
      return make_twop(x, " + ", y);
    case MINUS: 
      return make_twop(x, " - ", y);
    case MULT: 
      return make_twop(x, " \\cdot ", y);
    case FRAC:
      buf = malloc(MAX_STRING);
      buf[0]='\0';
      assert(buf != NULL);
      strcat(buf, "\\frac{");
      strcat(buf, x);
      strcat(buf, "}{");
      strcat(buf, y);
      strcat(buf, "}");
      return buf;
    case POW:
      buf = malloc(MAX_STRING);
      buf[0]='\0';
      assert(buf != NULL);
      strcat(buf, "{");
      strcat(buf, x);
      strcat(buf, "}^{");
      strcat(buf, y);
      strcat(buf, "}");
      return buf;
    default: 
      return strdup(x);
    }
}

// Функция для вывода числовой константы
char* print_func_const(const FUNCTION *func) {
  assert(func->type == CONST);

  char *buf = malloc(20);
  if (!buf) return NULL;

  sprintf(buf, "%lfI", cimag(func->num));
  return buf;
}
