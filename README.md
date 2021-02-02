# Priority Queue

Priority queue data structure micro library.

## Installation

The libraries and headers are installed according to `PREFIX` variable. 
The default value is `/usr/local/`. Libraries are copied to `$PREFIX/lib`, 
 and header file is copied to `$PREFIX/include`.

````
$ sudo make install
````

## Testing

There is a simple core logic test implemented in [tests.c](tests.c),

````
$ make test
````

and a memory allocation checking that uses also [tests.c](tests.c).

````
$ make memcheck
````

## Libraries

Examples of inclusion of library `prioqueue` in programs:

- static
````
$ gcc tests.c -o tests /usr/local/lib/libprioqueue.a
````

- dynamic
````
gcc  tests.c -o tests -lprioqueue
````

## Using

An example of a library client is as follows:

````C
#include <stdio.h>
#include <prioqueue.h>

typedef struct kv_struct {
  long key;
  long prio;
} KV;

static void print_prio(KV *kv) {
  printf("prio=%ld\n", kv->prio);
}

int cmp(void *x, void *y) {
  KV *u = (KV*)x, v = *(KV*)y;
  return u->prio - v->prio;
}

#define N 3
int main(int argc, char **argv) {
  int i;
  PrioQueue *pq;
  KV *kv, kvs[N] = {{1, 30}, {2, 20}, {3, 10}};
  
  /* Lower numbers first */
  pq = prioqueue_new(cmp, PQMin);

  for (i = 0; i < N; i++)
    prioqueue_insert(pq, &kvs[i]);

  /* Get and print the least prio but maintain it. */
  print_prio((KV*)prioqueue_peek());

  /* Delete and print the prio values in descending order */
  for (i = 0; i < N; i++) {
    kv = (KV*)prioqueue_delete(pq);
    print_prio(kv);
  }

  /* Release memory */
  prioqueue_free(pq);
}
````

## References

- ["Algorithms" (4th Edition)](https://www.amazon.com/Algorithms-4th-Robert-Sedgewick/dp/032157351X/ref=sr_1_1?dchild=1&keywords=sedgewick&qid=1612210852&sr=8-1) 
  by Robert Sedgewick, Kevin Wayne. Addison-Wesley Professional, 2011.
- ["Programming Pearls" (2nd Edition)](https://www.amazon.com/Programming-Pearls-2nd-Jon-Bentley/dp/0201657880/ref=sr_1_1?dchild=1&keywords=programming+pearls&qid=1612210954&sr=8-1)
  by Jon Bentley. Addison-Wesley Professional, 1999.
- ["Mastering Algorithms with C: Useful Techniques from Sorting to Encryption" (1st edition)](https://www.amazon.com/Mastering-Algorithms-C-Kyle-Loudon/dp/1565924533)
  by Kyle Loudon. O'Reilly Media, 1999.
