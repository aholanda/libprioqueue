# Priority Queue

Priority queue data structure micro library.

## Installation

The libraries, header and man pages are installed according to `PREFIX` variable. 
The default value is `/usr/local/`. Libraries are copied to `$PREFIX/lib`, 
 header file is copied to `$PREFIX/include`, and man pages are copied 
 to `$PREFIX/man/man3`.

````sh
$ make # Optional, but needed if test and memory allocation checking is to performed.
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

## Documentation
After installing the micro-library, there's available 
man pages for all exported functions. For example, 
to access information about `prioqueue_new()` function, 
execute:

````sh
$ man prioqueue_new
````

The source code is well documented to explore 
implementation details.

## Using

An example of a client program is as follows:

````C
#include <stdio.h>
#include <prioqueue.h>

typedef struct kv_struct {
  long key;
  long prio;
} KV;

static void print_kv(KV *kv, char *op) {
  printf("%s: {key=%ld, prio=%ld}\n", op, kv->key, kv->prio);
}

static int cmp(void *x, void *y) {
  KV *u = (KV*)x, *v = (KV*)y;
  return u->prio - v->prio;
}

#define N 3
int main(int argc, char **argv) {
  int i;
  PrioQueue *pq;
  KV *kv, kvs[N] = {{1, 30}, {2, 20}, {3, 10}};
  
  /* Lower numbers first. To insert greater numbers first, change PQMin to PQMax. */
  pq = prioqueue_new(cmp, PQMin);

  /* Insert the KVs and rearrange the queue properly. */
  for (i = 0; i < N; i++) {
    kv = &kvs[i];
    prioqueue_insert(pq, &kvs[i]);
    print_kv(kv, "insert");
  }

  printf("#elements inserted: %ld\n", prioqueue_size(pq));

  /* Get and print the KV with the least prio but maintain it. */
  print_kv((KV*)prioqueue_peek(pq), "peek");

  /* Delete and print the KVs with prio values in ascending order */
  for (i = 0; i < N; i++) {
    if (!prioqueue_is_empty(pq)) {
      kv = (KV*)prioqueue_delete(pq);
      print_kv(kv, "delete");
    }
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
