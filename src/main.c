#include <stdio.h>
#include <stdlib.h>

#include "functions.h"
#include "tree.h"
#include "random_tree.h"
#include "random_func.h"
#include "solve.h"


#if 0
int main(){
    FUNCTION *f;
    NODE *tree, *tmp, *tmp2;
    char *res;

    // Создаём корень: cos(...)
    f = create_func_twop(PLUS);
    tree = create_tree(f);

    // Левое поддерево: sin(x) + e^(3 - 2)
    f = create_func_onep(SIN);
    add_left(tree, f);

    // Левый потомок: sin(x)
    tmp = get_left(tree);
    f = create_func_const(6);
    add_left(tmp, f);
    add_left(get_left(tmp), create_func_const(0)); // sin(x) → sin(0), x подставляется на месте

    // Правый потомок: e^(3 - 2)
    f = create_func_const(1);
    add_right(tmp, f);

    // Вложенное поддерево: 3 - 2
    tmp2 = get_right(tmp);
    f = create_func_twop(MINUS);
    add_left(tmp2, f);
    add_left(get_left(tmp2), create_func_const(3));
    add_right(get_left(tmp2), create_func_const(2));

    // Правое поддерево: -5
    f = create_func_onep(COS);
    add_right(tree, f);

    // Вычисляем строковое представление выражения
    //print_tree(tree, 5);
    res = get_an_fun(tree, "0");
    
    printf("%s\n", res);
    res = get_an_fun(tree, "0");
    printf("%s\n", res);
    res = get_an_fun(tree, "0");
    printf("%s\n", res);
    res = get_an_fun(tree, "0");
    printf("%s\n", res);

    // Освобождаем память
    delete_tree(tree);
    free(res);

    return 0;
}
#endif
#if 1
int main(){
  NODE* tree;
  char* res;

  tree = get_random_tree(3);
  get_random_func_tree(tree);
  //tree = create_tree(create_func_onep(SIN));
  //add_left(tree, create_func_onep(X));
 // add_right(tree, create_func_onep(SIN));
  res = get_an_fun(tree, "x");
  printf("\n\n%s\n", res);

  save_points_to_file("output.csv", tree, 0, 0, 2, 8000);
  free(res);
  return 0;
}
#endif
