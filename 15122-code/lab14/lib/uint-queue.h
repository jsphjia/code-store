/*
 * Queues of unsigned integers
 *
 * 15-122 Principles of Imperative Computation */

#include <stdbool.h>

#ifndef UINT_QUEUE_H
#define UINT_QUEUE_H

/*********************/
/* Client interface */
/*********************/

typedef void uint_queue_print_fn(unsigned int x);

/*********************/
/* Library interface */
/*********************/

// typedef ______* queue_t;
typedef struct queue_header *queue_t;

bool queue_empty(queue_t Q)
/*@requires Q != NULL; @*/ ;

queue_t queue_new()
/*@ensures \result != NULL; @*/ ;

void enq(queue_t Q, unsigned int x)
/*@requires Q != NULL; @*/ ;

unsigned int deq(queue_t Q)
/*@requires Q != NULL && !queue_empty(Q); @*/ ;

void queue_print(queue_t Q, uint_queue_print_fn* elem_print)
/*@requires Q != NULL && elem_print != NULL; */ ;

void queue_free(queue_t Q)
/*@requires Q != NULL; @*/ ;

#endif // UINT_QUEUE_H
