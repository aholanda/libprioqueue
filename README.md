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

## References

- ["Algorithms" (4th Edition)](https://www.amazon.com/Algorithms-4th-Robert-Sedgewick/dp/032157351X/ref=sr_1_1?dchild=1&keywords=sedgewick&qid=1612210852&sr=8-1) 
  by Robert Sedgewick, Kevin Wayne. Addison-Wesley Professional, 2011.
- ["Programming Pearls" (2nd Edition)](https://www.amazon.com/Programming-Pearls-2nd-Jon-Bentley/dp/0201657880/ref=sr_1_1?dchild=1&keywords=programming+pearls&qid=1612210954&sr=8-1)
  by Jon Bentley. Addison-Wesley Professional, 1999.
  