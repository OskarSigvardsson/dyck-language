
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "random_tree.h"

const char *parse(const char *str, char term) {

	char c;
	while ((c = *str++)) {
		
		switch (c) {

		case '(': str = parse(str, ')'); break;
		case '[': str = parse(str, ']'); break;
		case '{': str = parse(str, '}'); break;

		case ')': return term == ')' ? str : NULL;
		case ']': return term == ']' ? str : NULL;
		case '}': return term == '}' ? str : NULL;

		}

		if (!str) return NULL;
	}

	// you only get here if you've reached the null terminator, and so the
	// string is valid if term == '\0'
	return term == '\0' ? str : NULL;
}

bool recdes(const char *str) {
	const char *ptr = parse(str, '\0');

	return ptr != NULL;
}


bool check(const char *str) {
    struct elem {
        char c;
        struct elem *next;
    };

    struct elem *stack = NULL;
    bool success;
   
#define PUSH(C)                                         \
    do {                                                \
        struct elem *new = malloc(sizeof(struct elem)); \
        new->c = C;                                     \
        new->next = stack;                              \
        stack = new;                                    \
    } while (0)
   
#define POP(C)									\
    do {										\
        if (stack == NULL || stack->c != C) {	\
            success = false;					\
            goto end;							\
        } else {								\
            struct elem *old = stack;			\
            stack = old->next;					\
            free(old);							\
        }										\
    } while (0)
   
    char c;
    while ((c = *str++)) {
        switch (c) {
        case '(': PUSH(')'); break;
        case '{': PUSH('}'); break;
        case '[': PUSH(']'); break;
        case ')': POP(')');  break;
        case '}': POP('}');  break;
        case ']': POP(']');  break;
        }
    }

    success = stack == NULL;
#undef PUSH
#undef POP

end:
    while (stack) {
        struct elem *old = stack;
        stack = old->next;
        free(old);
    }

    return success;
}

bool test() {
	bool success = true;
#define TEST(pass, str) do {										\
		bool c1 = check(str);										\
		bool c2 = recdes(str);										\
		printf("%i %i %i %s\n", c1 == pass, c2 == pass, pass, str);	\
		success &= pass == c1;										\
		success &= pass == c2;										\
	} while (0)

    TEST(1, "(){}[()]");
    TEST(0, "(){}}[()]");
    TEST(0, "){}[()]");
    TEST(1, "");
    TEST(1, "()()()()");
    TEST(1, "()");
    TEST(1, "{{}}()[[[(())()]]]");
    TEST(0, "{{}}()[[(())()]]]");
    TEST(0, "(((");
    TEST(1, "(((nonsense)))");
#undef TEST

	return success;
}

int main(int argc, char **argv) {
	if (argc < 2) return 1;
	
	if (0 == strcmp(argv[1], "test")) {
		return 1 - test();
	} else if (0 == strcmp(argv[1], "make-tree")) {
		if (argc < 4) return 1;

		char *endptr;
		int chunks = strtol(argv[2], &endptr, 10);

		if (endptr == argv[1] || *endptr != '\0') {
			printf("Invalid number %s", argv[1]);
			return 1;
		}

		int size = strtol(argv[3], &endptr, 10);

		if (endptr == argv[1] || *endptr != '\0') {
			printf("Invalid number %s", argv[2]);
			return 1;
		}
		
		//srand(time(0));

		for (int chunk = 0; chunk < chunks; chunk++) {
			fprintf(stderr, "Making chunk %d...\n", chunk);
			tree *tree = tree_make(size);
			tree_print(tree, stdout);
			tree_free(tree);
		}

		return 0;
	} 
}
