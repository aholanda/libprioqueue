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

void prioqueue_free(PrioQueue *pq) {
        if (pq) {
                if (pq->heap)
                        free(pq->heap);

                free(pq);
        }
}