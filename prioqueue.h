/* 
  Copyright (c) 2021 LibPrioQueue 
  License: MIT
*/
#ifndef __LIBPRIOQUEUE_PRIOQUEUE_H__
#define __LIBPRIOQUEUE_PRIOQUEUE_H__

/* Initial capacity for the heap. */
#ifndef PRIOQUEUE_INITIAL_CAPACITY
#define PRIOQUEUE_INITIAL_CAPACITY 64
#endif

typedef struct prioqueue_struct {
    /* Number of elements */
    long n;
    /* Function used to compare the elements */
    int (*cmp)(void *x, void *y);
    /* Internal function used to compare the elements */
    int (*__cmp)(struct prioqueue_struct *pq, void *x, void *y);
   /* Heap-ordered complete binary tree implemented as array */
    void **heap;
    /* Capacity: number of elements that can be inserted 
       in the heap array. */
    long cap;
} PrioQueue;

enum prioqueue_order {PQMin, PQMax};

extern PrioQueue *prioqueue_new(int (*cmp)(void *x, void *y),
                                enum prioqueue_order order);
extern void prioqueue_insert(PrioQueue *pq, void *x);
extern void *prioqueue_peek(const PrioQueue *pq);
extern void *prioqueue_delete(PrioQueue *pq);
extern int prioqueue_is_empty(const PrioQueue *pq);
extern long prioqueue_size(const PrioQueue *pq);
extern void prioqueue_free(PrioQueue *pq);
#endif