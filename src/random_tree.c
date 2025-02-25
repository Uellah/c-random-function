#include "random_tree.h"
#include "tree.h"
#include <stdlib.h>
#include <time.h>

int success(){return ((double)rand() / RAND_MAX) < SUCCESS ? 1 : 0; }

void helper_get_random_tree(NODE* root, int cur_depth, const int depth){
  if (cur_depth == depth) return;
  
  if (success()){
    add_left(root, NULL);
    helper_get_random_tree(get_left(root), cur_depth + 1, depth);
  }

  if (success()){
    add_right(root, NULL);
    helper_get_random_tree(get_right(root), cur_depth + 1, depth);
  }

  return;
}

NODE* get_random_tree(const int depth){
  srand(time(NULL));
  NODE *res;
  res = create_tree(NULL);

  helper_get_random_tree(res, 0, depth);
  
  return res;
}
