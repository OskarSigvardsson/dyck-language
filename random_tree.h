#pragma once

#include <stdio.h>

typedef struct tree tree;

tree *tree_make(int size);
void tree_print(tree *tree, FILE *f);
void tree_free(tree *tree);
