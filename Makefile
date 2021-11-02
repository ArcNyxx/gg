# gridguess - classroom board game
# Copyright (C) 2021 FearlessDoggo21
# see LICENCE file for licensing information

.POSIX:

include config.mk

SRC = $(wildcard *.c)
HEAD = $(wildcard *.h)
OBJ = $(SRC:.c=.o)

all: options gg

options:
	@echo gridguess build options:
	@echo "CFLAGS  = $(GGCFLAGS)"
	@echo "LDFLAGS = $(GGLDFLAGS)"
	@echo "CC      = $(CC)"

.c.o:
	$(CC) $(GGCFLAGS) -c $<

$(OBJ): $(HEAD) config.mk

gg: $(OBJ)
	$(CC) -o $@ $(OBJ) $(GGLDFLAGS)

clean:
	rm -f gg $(OBJ) gg-$(VERSION).tar.gz

dist: clean
	mkdir -p gg-$(VERSION)
	cp -R LICENCE Makefile README config.mk \
		$(SRC) $(HEAD) gg-$(VERSION)
	tar -cf - gg-$(VERSION) | gzip > gg-$(VERSION).tar.gz
	rm -rf gg-$(VERSION)

install: gg
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f gg $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/gg

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/gg

.PHONY: all options clean dist install uninstall
