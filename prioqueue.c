/* 
  Copyright (c) 2021 LibPrioQueue 
  License: MIT
*/

#include <assert.h>
#include <stdlib.h>

#include "prioqueue.h"

/*
  swap exchanges the ith position to the jth position 
  in the queue heap array.
*/
static void swap(PrioQueue *pq, const long i, const long j) {
        void *tmp = NULL;

        assert(pq);
     
        tmp = pq->heap[i];
        pq->heap[i] = pq->heap[j];
        pq->heap[j] = tmp;
}

/*
  When PQMin order is chosen, the function is_less_than
  is used to test if x is less than y with the help 
  of cmp function assigned to PrioQueue.cmp.
*/
static int is_less_than(PrioQueue *pq, void *x, void *y) {
        return pq->cmp(x, y) < 0;
}

/*
  When PQMax order is chosen, the function is_greater_than
  is used to test if x is greater than y with the help 
  of cmp function assigned to PrioQueue.cmp.
*/
static int is_greater_than(PrioQueue *pq, void *x, void *y) {
        return pq->cmp(x, y) > 0;
}

/*
  Create a priority queue structure that uses 
  function pointer cmp to compare the elements 
  in the heap array. The enumeration order sets
  if the queue is in ascending order (PQMin) or
  descending order (PQMax). The value PQMin for 
  the enumeration means that lower values have 
  greater priorities, and PQMax value the opposite.

  During the structure creation, a heap array is allocated 
  with a initial size equals to the macro 
  PRIOQUEUE_INITIAL_CAPACITY set in the header. 
  This avoids frequent reallocations and this value 
  may be modified adjusting each project needs.

  The first element in the heap array is ignored.
  This becomes the shifts in the heap easier to 
  reasoning. (Thanks to Sedgewick/Wayne)
  The elements are numbered from 1 up to N (included).

  @return a pointer to priority queue structure is returned.
*/
PrioQueue *prioqueue_new(int (*cmp)(void *x, void *y),
                         enum prioqueue_order order) {
        PrioQueue *pq;

        pq = malloc(sizeof(PrioQueue));
        assert(pq);
        pq->n = 0;
        assert(cmp);        
        pq->cmp = cmp;
        if (order == PQMax)
                pq->__cmp = is_greater_than;
        else
                pq->__cmp = is_less_than;

        /* Allocate the unused element. */
        pq->cap = PRIOQUEUE_INITIAL_CAPACITY;
        pq->heap = (void**)calloc(pq->cap, sizeof(void*));
        assert(pq->heap);
        pq->heap[0] = NULL;

        return pq;
}

/*
  realoc_heap reallocate memory for priority queue
  pq using new_capacity to represents the maximum 
  number of elements minus 1 that can be stored in 
  the heap array.
*/
static void **realloc_heap(PrioQueue *pq, long new_capacity) {
        void **tmp;
        
        tmp = (void**)realloc(pq->heap, 
                              new_capacity * sizeof(void*));
        assert(tmp);
        pq->heap = tmp;
        pq->cap = new_capacity;

        return pq->heap;
}
/*
  siftup compares the ith element of tree heap array 
  with its parent, swapping positions if the 
  heap invariance is not obeyied.
*/
static void siftup(PrioQueue *pq, long i) {
        void *x, *y;

        while (i > 1) {
                x = pq->heap[i/2];
                y = pq->heap[i];

                if (pq->__cmp(pq, x, y))
                        break;

                swap(pq, i/2, i);
                i /= 2;
        }
}

/*
  To avoid constant reallocations of memory for the 
  heap array, when there is no more room for a new 
  element the capacity of heap array is doubled.

  prioqueue_insert assign the element pointing to 
  elem at the end of the heap array. Then siftup 
  is called to restore the heap invariance in the 
  array.
*/
void prioqueue_insert(PrioQueue *pq, void *elem) {
        assert(pq);

        /* 
          When the number of elements plus one (unused element) 
          is greater than the heap array capacity, the 
          heap array is increased by a factor of two.
        */
        if (pq->n+2 == pq->cap) 
                pq->heap = realloc_heap(pq, pq->cap*2);
        
        pq->heap[++pq->n] = elem;
        siftup(pq, pq->n);
}

/* 
   siftdown function compares the ith element with
   its children in the tree heap array checking if 
   the heap invariance is obeyed. If invariance is not
   obeyed ith is swapped with the child that breaks
   the invariance and ith takes jth place. The process
   continues until the invariance is enforced in the tree
   heap array.
*/
static void siftdown(PrioQueue *pq, long i) {
        long j;
        void *x, *y;

        while (2*i <= pq->n) {
                j = 2*i;

                x = pq->heap[i];
                y = pq->heap[j];

                if (j < pq->n && !pq->__cmp(pq, x, y))
                        j++;

                y = pq->heap[j];
                if (pq->__cmp(pq, x, y))
                        break;

                swap(pq, i, j);
                i = j;
        }
}

/*
  prioqueue_delete returns the first element
  in the heap array (PQMin: lower value, PQMax: greater value)
  removing it from array by assigning the last element
  to 1st position. The siftdown() is called passing the 
  1st position as argument to restore the heap invariant.

  Remember: 0th element is unused.
*/
void *prioqueue_delete(PrioQueue *pq) {
        void *elem;

        assert(pq);
        assert(pq->n > 0);

        elem = pq->heap[1];
        swap(pq, 1, pq->n--);
        pq->heap[pq->n+1] = NULL;        
        siftdown(pq, 1);

        /* 
          When the number of elements plus one (unused element) 
          is less than the half of the heap array capacity, the 
          heap array is shrinked by a factor of two.
        */
        if (pq->n+1 < pq->cap/2)
                pq->heap = realloc_heap(pq, pq->cap/2);

        return elem;
}

/*
  prioqueue_peek returns the first element
  in the heap array (PQMin: lower value, PQMax: greater value)
  without removing it from the array.
*/
void *prioqueue_peek(const PrioQueue *pq) {
        assert(pq);
        assert(pq->n > 0);

        return pq->heap[1];
}

/*
  prioqueue_is_empty returns true (1) if the 
  heap array is empty or false (0), otherwise.
*/
int prioqueue_is_empty(const PrioQueue *pq) {
        assert(pq);

        return pq->n == 0;
}

/*
  prioqueue_size returns the number of elements
  in the heap array.
*/
long prioqueue_size(const PrioQueue *pq) {
        assert(pq);

        return pq->n;
}
/*
  prioqueue_free releases all the memory 
  allocated to PrioQueue structure, including
  the heap array.
*/
void prioqueue_free(PrioQueue *pq) {
        if (pq) {
                if (pq->heap)
                        free(pq->heap);

                free(pq);
        }
}