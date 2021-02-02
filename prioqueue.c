#include <assert.h>
#include <stdlib.h>

#include "prioqueue.h"

static void swap(PrioQueue *pq, const long i, const long j) {
        void *tmp = NULL;

        assert(pq);
     
        tmp = pq->heap[i];
        pq->heap[i] = pq->heap[j];
        pq->heap[j] = tmp;
}

static int is_less_than(PrioQueue *pq, void *x, void *y) {
        return pq->cmp(x, y) < 0;
}

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
  element the capacity of heap array is doubled, 
*/
void prioqueue_insert(PrioQueue *pq, void *elem) {
        assert(pq);

        if (pq->n+2 == pq->cap) 
                pq->heap = realloc_heap(pq, pq->cap*2);
        
        pq->heap[++pq->n] = elem;
        siftup(pq, pq->n);
}

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

void *prioqueue_delete(PrioQueue *pq) {
        void *elem;

        assert(pq);
        assert(pq->n > 0);

        elem = pq->heap[1];
        swap(pq, 1, pq->n--);
        pq->heap[pq->n+1] = NULL;        
        siftdown(pq, 1);

        if (pq->n+1 < pq->cap/2)
                pq->heap = realloc_heap(pq, pq->cap/2);

        return elem;
}

void *prioqueue_peek(const PrioQueue *pq) {
        assert(pq);
        assert(pq->n > 0);

        return pq->heap[1];
}

int prioqueue_is_empty(const PrioQueue *pq) {
        assert(pq);

        return pq->n == 0;
}

long prioqueue_size(const PrioQueue *pq) {
        assert(pq);

        return pq->n;
}
/*
  prioqueue_free releases all the memory 
  allocated to the structure.
*/
void prioqueue_free(PrioQueue *pq) {
        if (pq) {
                if (pq->heap)
                        free(pq->heap);

                free(pq);
        }
}