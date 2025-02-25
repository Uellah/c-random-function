#include "tree.h"
#include "functions.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

struct Node {
 FUNCTION *func;
 struct Node *left;
 struct Node *right;
};

NODE* create_tree(FUNCTION *fun){
  NODE* res;
  res = calloc(1, sizeof(NODE));
  res->func = fun;
  res->left = NULL;
  res->right = NULL;
  return res;
}

int add_left(NODE *root, FUNCTION *fun){
  if (root == NULL) return 0;
  NODE *tmp = calloc(1, sizeof(NODE));
  if (tmp == NULL) return 0;
  tmp->func = fun;
  tmp->left = NULL;
  tmp->right = NULL;
  
  root->left = tmp;
  return 1;
}

int add_right(NODE *root, FUNCTION *fun){
  if (root == NULL) return 0;
  NODE *tmp = calloc(1, sizeof(NODE));
  if (tmp == NULL) return 0;
  tmp->func = fun;
  tmp->left = NULL;
  tmp->right = NULL;
  
  root->right = tmp;
  return 1;
}

double complex evaluate(NODE *root, double complex x){
  double complex l_tmp, r_tmp;
  if (root == NULL){
    return 0;
  }

  switch(get_func_type(root->func)){
    case CONST:
      return evaluate_func_const(root->func);

    case ONE_P:
      if (root->left != NULL){
        l_tmp = evaluate(root->left, x);
        return evaluate_func_onep(root->func, l_tmp);
      }

      else if (root->right != NULL){
        l_tmp = evaluate(root->right, x);
        return evaluate_func_onep(root->func, l_tmp); 
      }

      else return evaluate_func_onep(root->func, x);
    
    case TWO_P:
      l_tmp = evaluate(root->left, x);
      r_tmp = evaluate(root->right, x);
      return evaluate_func_twop(root->func, l_tmp, r_tmp);

    default:
      return 0;
    }
}

NODE* get_left(NODE* root){
  if (root == NULL) return NULL;
  return root->left;
}

NODE* get_right(NODE* root){
  if (root == NULL) return NULL;
  return root->right;
}

void change_func(NODE* root, FUNCTION* fun){
  root->func = fun;
}

// Рекурсивный вывод дерева в красивом формате
void print_tree(NODE *root, int space) {
    char x[2] = {" "};
    if (root == NULL) return;
    
    // Увеличиваем отступ (чтобы правые узлы были дальше справа)
    space += 10;
    
    // Сначала печатаем правого потомка
    print_tree(root->right, space);

    // Печатаем текущий узел с отступом
    printf("\n");
    for (int i = 10; i < space; i++) {
        printf(" ");
    }

    switch(get_func_type(root->func)){
      case ONE_P:
        printf("%s", print_func_onep(root->func, x));
        break;
      case TWO_P:
        printf("%s", print_func_twop(root->func, x, x));
        break;
      case CONST:
        printf("%s", print_func_const(root->func));
        break;
    }

    // Затем печатаем левого потомка
    print_tree(root->left, space);
}


void delete_tree(NODE* root){
  if (root == NULL) return;

  delete_tree(root->left);
  delete_tree(root->right);
  
  free(root->func);
  free(root);
}

// Основная функция для обхода дерева и вывода функций
char* get_an_fun(NODE *root, const char* x) {
    if (root == NULL) return strdup("0"); // Возвращаем копию строки

    char *l_tmp = NULL, *r_tmp = NULL, *result = NULL;

    switch(get_func_type(root->func)) {
        case CONST:
            return print_func_const(root->func);

        case ONE_P:
            if (root->left)
            {
              l_tmp = get_an_fun(root->left, x);
              result = print_func_onep(root->func, l_tmp);
            }
            else if (root->right)
            {
              l_tmp = get_an_fun(root->right, x);
              result =  print_func_onep(root->func, l_tmp);
            }
            else
            {
              l_tmp = malloc(MAX_STRING);
              strcpy(l_tmp, x);
              result = print_func_onep(root->func, l_tmp);
            }
            free(l_tmp);
            return result;

        case TWO_P:
            assert(root->left && root->right);
            l_tmp = get_an_fun(root->left, x);
            r_tmp = get_an_fun(root->right, x);
            result = print_func_twop(root->func, l_tmp, r_tmp);
            free(l_tmp);
            free(r_tmp);
            return result;

        default:
            return strdup("0");
    }
}
