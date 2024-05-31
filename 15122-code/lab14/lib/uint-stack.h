/*
 * Stacks of unsigned integers
 *
 * 15-122 Principles of Imperative Computation */

#include <stdbool.h>

#ifndef UINT_STACK_H
#define UINT_STACK_H

/*********************/
/* Client interface */
/*********************/

typedef void uint_stack_print_fn(unsigned int x);

/*********************/
/* Library interface */
/*********************/

// typedef ______* stack_t;
typedef struct stack_header *stack_t;

bool stack_empty(stack_t S)
/*@requires S != NULL; @*/ ;

stack_t stack_new()
/*@ensures \result != NULL; @*/ ;

void push(stack_t S, unsigned int x)
/*@requires S != NULL; @*/
/*@ensures !stack_empty(S); @*/ ;

unsigned int pop(stack_t S)
/*@requires S != NULL && !stack_empty(S); @*/ ;

size_t stack_size(stack_t S)
/*@requires S != NULL; @*/ ;

void stack_print(stack_t S, uint_stack_print_fn* elem_print)
/*@requires S != NULL && elem_print != NULL; @*/ ;

void stack_free(stack_t S)
/*@requires S != NULL; @*/ ;

#endif // UINT_STACK_H
