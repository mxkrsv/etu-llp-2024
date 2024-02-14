CROSS_COMPILE ?= x86_64-w64-mingw32-
CC = gcc
WINE ?= wine64
WINEDEBUG ?= "fixme-all"
PROG ?= main

build: $(PROG).c
	$(CROSS_COMPILE)$(CC) $(PROG).c -o $(PROG)

clean:
	rm -f $(PROG).exe

run:
	WINEDEBUG=$(WINEDEBUG) $(WINE) $(PROG).exe

.PHONY: clean run
