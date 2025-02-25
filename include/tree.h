#ifndef TREE_H
#define TREE_H

#include "types.h"

NODE* create_tree(FUNCTION *fun);

int add_left(NODE *root, FUNCTION *fun);
int add_right(NODE *root, FUNCTION *fun);

NODE* get_left(NODE* root);
NODE* get_right(NODE* root);
void change_func(NODE* root, FUNCTION* fun);

double complex evaluate(NODE *root, double complex x);
char* get_an_fun(NODE *root, const char* x);

void delete_tree(NODE* root);

void print_tree(NODE *root, int level);

#endif
