#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "functions.h"
#include "tree.h"
#include "random_tree.h"
#include "random_func.h"
#include "solve.h"
#include "fractal_drawer.h"

int main(){
  init();
  NODE* func;
  char* res;

  func = get_random_tree(3);
  get_random_func_tree(func);

  res = get_an_fun(func, "x");
  printf("\n\n%s\n", res);

  run_fractal_loop(func);
  return 0;
}


















#if 0
int main(){
  NODE* tree;
  char* res;
  double complex a = 2 * I;
  //tree = get_random_tree(3);
  //get_random_func_tree(tree);
  //tree = create_tree(create_func_twop(FRAC));

  //add_left(tree, create_func_onep(X));
  // add_right(tree, create_func_onep(SIN));
  //tree = create_tree(create_func_twop(FRAC));  // Корень: деление

  // Левый потомок (вычитание: x - conj(x))
  //add_left(tree, create_func_twop(MINUS));  

  // Левый узел вычитания (x)
  //add_left(get_left(tree), create_func_onep(X));  

  // Правый узел вычитания (conj(x))
  //add_right(get_left(tree), create_func_onep(CONJ));  

  /// Правый потомок корня (константа 2i)
  //add_right(tree, create_func_const(a));  
  tree = create_tree(create_func_twop(MINUS));  // Корень: вычитание

  // Левый потомок (умножение: x * conj(x))
  add_left(tree, create_func_twop(MULT));  

  // Левый узел умножения (x)
  add_left(get_left(tree), create_func_onep(X));  

  // Правый узел умножения (conj(x))
  add_right(get_left(tree), create_func_onep(CONJ));  

  // Правый потомок корня (константа r)
  add_right(tree, create_func_const(0.01));  

  res = get_an_fun(tree, "x");
  printf("\n\n%s\n", res);
  //printf("%lf", evaluate(tree, a));
  save_points_to_file("output.csv", tree, 0, 0, 0.5, 9000);
  free(res);
  return 0;
}
#endif

