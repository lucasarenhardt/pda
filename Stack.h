#ifndef STACK_H
#define STACK_H
#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
    char data;
    struct Node *next;
}stackNode;

typedef struct Stack{
    stackNode *top;
}stack;

stack *createstack();
int isEmpty(stack *s);
void push(stack *s, char data);
char pop(stack *s);
void empty(stack *s);
char *stackToString(stack *s);

#endif //STACK_H