#include "random_func.h"
#include <stdlib.h>
#include <time.h>
#include "tree.h"
#include "functions.h"

FUNC_ONE_PARAM rand_func_onep(){
  return rand() % END_ONE_P;
}

FUNC_TWO_PARAM rand_func_twop(){
  return rand() % END_TWO_P;
}

double complex rand_func_const(){
  return (double complex)(rand() % 1000 + 2) ;
}

FUNCTION* get_func_to_node(NODE *root){
  // при вызове должен быть инициализирован seed()
  int leafs;
  FUNCTION* fun;

  leafs = (get_left(root) != NULL) + (get_right(root) != NULL);
  switch(leafs){
    case 0:
      //if (rand() % 2){
      //fun = create_func_onep(rand_func_onep());
     // }
     // else {
  //      fun = create_func_const(rand_func_const());
  //    }
    //  break;
    case 1:
      fun = create_func_onep(rand_func_onep());
      break;
    case 2:
      fun = create_func_twop(rand_func_twop());
      break;
  }
  return fun;
}

void get_random_func_tree_helper(NODE* root){
  if (root == NULL) return;
  
  change_func(root, get_func_to_node(root));
  get_random_func_tree_helper(get_left(root));
  get_random_func_tree_helper(get_right(root));

  return;
}

void get_random_func_tree(NODE* root){
  srand(time(NULL));

  get_random_func_tree_helper(root);
  return;
}
