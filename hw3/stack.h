/*
 * stack.h
 *
 *  Created on: Apr 1, 2014
 *      Author: armin
 */
#include <stdlib.h>
#include <stdio.h>

#ifndef STACK_H_
#define STACK_H_

struct StackNode;

typedef struct StackNode {
    struct StackNode *next;
    void *value;
} StackNode;

typedef struct Stack {
    int count;
    int limit;
    StackNode *head;
} Stack;

Stack *createStack();
void *pop(Stack *stack);
void push(Stack *stack, void *value);
void *peek(Stack *stack, int key);
void destroyStack(Stack *stack);
void printStack(Stack *stack);

#endif /* STACK_H_ */
