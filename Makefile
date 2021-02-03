CC := clang
CFLAGS := -Wall -g
ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

all: libprioqueue.a libprioqueue.so prioqueue.h
phony += all

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
trash += libprioqueue.so

install: libprioqueue.a libprioqueue.so ./man3/install.sh
	@install -d $(PREFIX)/lib/
	@install -m 644 libprioqueue.a $(PREFIX)/lib/
	@install -m 644 libprioqueue.so $(PREFIX)/lib/	
	@install -d $(PREFIX)/include/
	@install -m 644 prioqueue.h $(PREFIX)/include/
	@./man3/install.sh --prefix $(PREFIX) --install
phony += install

uninstall: clean
	$(RM) $(PREFIX)/lib/libprioqueue.a 
	$(RM) $(PREFIX)/lib/libprioqueue.so 	
	$(RM) $(PREFIX)/include/prioqueue.h
	./man3/install.sh --prefix $(PREFIX) --uninstall	
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
	$(RM) $(trash) *.o *.3

.PHONY: $(phony)
