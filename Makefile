CC := clang
CFLAGS := -Wall -g
ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

help:
	@echo "Options are:"
	@echo "install:\tcopy the shared and static libraries to"
	@echo "\t\t$(PREFIX)/lib and headers to $(PREFIX)/include."
	@echo "uninstall:\tdelete the shared and static libraries from"
	@echo "\t\t$(PREFIX)/lib and headers from $(PREFIX)/include."
	@echo "test:\t\tperform a simple test to check the core logic."
	@echo "memcheck:\tcheck memory allocation using tests and valgrind."
phony += help

libprioqueue.a: prioqueue.o
	ar rcs $@ $<
trash += libprioqueue.so

libprioqueue.so: prioqueue.o
	@$(CC) -shared -o $@ $<
	@ldconfig
trash += libprioqueue.so

install: libprioqueue.a libprioqueue.so
	@install -d $(PREFIX)/lib/
	@install -m 644 libprioqueue.a $(PREFIX)/lib/
	@install -m 644 libprioqueue.so $(PREFIX)/lib/	
	@install -d $(PREFIX)/include/
	@install -m 644 prioqueue.h $(PREFIX)/include/
phony += install

uninstall:
	$(RM) $(PREFIX)/lib/libprioqueue.a 
	$(RM) $(PREFIX)/lib/libprioqueue.so 	
	$(RM) $(PREFIX)/include/prioqueue.h
phony += uninstall

tests: tests.c prioqueue.o
	@$(CC) $(CFLAGS) $^ -o $@
thrash += tests

test: tests
	@./$<
phony += test

memcheck: tests
	if [ -x "`which valgrind`" ]; then\
		valgrind --track-origins=yes --show-leak-kinds=all --leak-check=full -s ./$<;\
	else\
		echo "Error: valgrind is not installed; please install it to check memory allocations.">&2;\
	fi
phony += memcheck

clean:
	$(RM) $(trash) *.o

.PHONY: $(phony)
