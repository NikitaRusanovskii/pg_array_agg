#include <stack.h>

void init_stack(stack* s) {
    memset(s, 0, sizeof(stack));
    s->top = -1;
}

void push(stack* s, T* value) {
    if (s->top < MAX - 1) s->stack[++s->top] = *value;
}

T* pop(stack* s) {
    return (s->top >= 0) ? &(s->stack[s->top--]) : NULL;
}
