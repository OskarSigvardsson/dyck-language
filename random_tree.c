#include "random_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

struct tree {
	char kind;
	struct tree *down;
	struct tree *right;
};

tree *tree_make(int size) {
	tree *leaves = malloc(size * sizeof(tree));

	const char *kinds = "([{";
	for (int i = 0; i < size; i++) {
		leaves[i].kind = kinds[rand() % 3];
		leaves[i].down = NULL;
		leaves[i].right = NULL;
	}

	tree *root = leaves;
	leaves += 1;
	
	int slots_capacity = size + 1;
	tree ***slots = malloc(slots_capacity * sizeof(tree**));

	int slots_size = 2;

	slots[0] = &(root->down);
	slots[1] = &(root->right);


	for (int i = 0; i < size - 1; i++) {
		tree *leaf = &leaves[i];
		int slot = rand() % slots_size;

		assert(slots_size < slots_capacity);

		*slots[slot] = leaf;
		slots[slot] = &leaf->down;
		slots[slots_size++] = &leaf->right;
	}

	free(slots);

	return root;
}

void tree_print(tree *tree, FILE *file) {
	if (!tree) return;
	
	putc(tree->kind, stdout);
	tree_print(tree->down, file);

	switch (tree->kind) {
	case '(': putc(')', stdout); break;
	case '[': putc(']', stdout); break;
	case '{': putc('}', stdout); break;
	default: assert(false);
	}

	tree_print(tree->right, file);
}

void tree_free(tree *tree) {
	free(tree);
}
