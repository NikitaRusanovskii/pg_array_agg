#pragma once
#include "utilfunc.h"

#define MAX 100
#define T ASTNode

typedef struct {
    T stack[MAX];
    int top;
} stack;

void init_stack(stack* s);
void push(stack* s, T value);
int pop(stack* s);
