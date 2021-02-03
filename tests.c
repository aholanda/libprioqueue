#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prioqueue.h"

typedef struct keyval_struct {
    long key;
    long prio;
} KV;

static int cmp_less(void *x, void *y) {
    KV *xx = NULL, *yy = NULL;

    assert(x && y);
    
    xx = (KV*)x;
    yy = (KV*)y;

    return xx->prio - yy->prio;
}

static int cmp_greater(void *x, void *y) {
    KV *xx = NULL, *yy = NULL;

    assert(x && y);
    
    xx = (KV*)x;
    yy = (KV*)y;

    return yy->prio - xx->prio;
}

static void assert_equal(void *x, void *y) {
    KV *xx = NULL, *yy = NULL;

    xx = (KV*)x;
    yy = (KV*)y;    
    
    if (x != y) {
        fprintf(stderr, "test failed %ld != %ld\n", 
                xx->prio, yy->prio);
        exit(EXIT_FAILURE);
    }
}

static void test(enum prioqueue_order order, long factor) {
    PrioQueue *pq;
    long i, delta, n = PRIOQUEUE_INITIAL_CAPACITY * factor;
    KV *kvs; /* array of key/prioues */
    KV *kv; /* pointer to kv struct */

    if (order == PQMin) {
        pq = prioqueue_new(cmp_less, PQMin);
        delta = pq->n;
    } else {
        pq = prioqueue_new(cmp_greater, PQMax);
        delta = 0;
    }

    kvs = (KV*)calloc(n, sizeof(KV));
    assert(kvs);
    for (i = 0; i < n; i++ ) {
        kvs[i].key = i;
        kvs[i].prio = labs(n-i) * 10;
        prioqueue_insert(pq, &kvs[i]);
    }

    for (i = n-1; i >= 0; i--) {
        if (!prioqueue_is_empty(pq)) {
            kv = (KV*)prioqueue_delete(pq);
            assert_equal(kv, &kvs[i]);
        }
    }
    if (kvs)
        free(kvs);
    prioqueue_free(pq);
}

static int tests() {
    int i;

    for (i = 0; i < 2; i++)
        test(i, 1);

    printf("ok: all tests passed\n");

    return EXIT_SUCCESS;
}

static int bench() {
    fprintf(stderr, "%s(): NOT IMPLEMENTED YET", __FUNCTION__);

    return EXIT_SUCCESS;
}

static int usage(char **argv) {
    fprintf(stderr, "usage: %s [--bench]\n", argv[0]);
    fprintf(stderr, "--bench: perform a benchmark;\n");
    fprintf(stderr, 
            "with no arguments, the program performs a simple test.");

    return EXIT_FAILURE;
}

int main(int argc, char**argv) {
    if (argc == 2
        && strncmp(argv[1], "--bench", sizeof("--bench"))==0)
        return bench();
    else if (argc==1)
        return tests();
    else
        return usage(argv);
}