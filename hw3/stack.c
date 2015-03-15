/*
 * stack.c
 *
 *  Created on: Apr 1, 2014
 *      Author: armin
 */

#include "stack.h"

Stack *createStack(unsigned int limit) {
	Stack *stack = calloc(1, sizeof(Stack));
	stack->count = 0;
	if (limit != 0)
		stack->limit = limit;
	return stack;
}

void *pop(Stack *stack) {
	StackNode *currentHead = stack->head;
	if (currentHead->next != NULL) {
		stack->head = currentHead->next;
	} else {
		if (stack->head != NULL) {
			currentHead = stack->head;
		}
		stack->head = NULL;
		free(stack->head);
	}
	stack->count--;
	free(currentHead);
	return currentHead->value;
}

void push(Stack *stack, void *value) {
	StackNode *newNode = calloc(1, sizeof(StackNode));
	newNode->next = stack->head;
	newNode->value = value;
	stack->head = newNode;

	if (stack->count == stack->limit) {
		StackNode *current = stack->head;
		StackNode *prev;
		while (current->next != NULL) {
			prev = current;
			current = current->next;
		}
		prev->next = NULL;
		free(current->value);
		free(current);
	} else {
		stack->count++;
	}
	return;
}

void *peek(Stack *stack, int key) {
	if (key > stack->count)
		return NULL;
	int i;
	int sCount = stack->count;
	StackNode *current = stack->head;
	for (i = 0; i < sCount; i++) {
		if (key == i) {
			return current->value;
		} else {
			current = current->next;
		}
	}
	return NULL;
}

void destroyStack(Stack *stack) {
	if (stack->count > 0) {
		StackNode *current = stack->head;

		if (current->next == NULL) {
			if (current != NULL) {
				free(current->value);

			}
			free(current);
		} else {
			while (current->next != NULL) {
				StackNode *nextNode = current->next;
				free(current->value);
				free(current);
				current = nextNode;
			}
			free(current->value);
			free(current);
		}
	}
	free(stack);
}

void printStack(Stack *stack) {
	int count = 0;
	while (count != stack->count) {
		char *command = peek(stack, count);
		printf("%d: %s\n", count + 1, command);
		count++;
	}
	return;
}

/*
int main(int argc, char **argv) {

	Stack *stack = createStack(3);
	push(stack, "zero");
	printf("Pushing zero\n");
	push(stack, "one");
	printf("Pushing one\n");
	push(stack, "two");
	printf("Pushing two\n");
	push(stack, "three");
	printf("Pushing three\n");
	printf("stack limit is %d\n", stack->limit);

	char *val;
	int i;
	int stackSize = stack->count;
	for (i = 0; i < stackSize; i++) {
		val = peek(stack, i);
		printf("Peeked %d is %s\n", i, val);
	}

	for (i = 0; i < stackSize; i++) {
		val = pop(stack);
		printf("Popped %d is %s\n", i, val);
	}

	printf("Destroying stack\n");
	destroyStack(stack);
	printf("Stack destroyed.\n");
	return 0;
}
*/
