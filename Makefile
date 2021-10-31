# gridguess - classroom board game
# Copyright (C) 2021 FearlessDoggo21
# see LICENCE file for licensing information

.POSIX:

include config.mk

SRC = $(wildcard *.c)
HEAD = $(wildcard *.h)
OBJ = $(SRC:.c=.o)

all: options gridguess

options:
	@echo gridguess build options:
	@echo "CFLAGS  = $(GGCFLAGS)"
	@echo "LDFLAGS = $(GGLDFLAGS)"
	@echo "CC      = $(CC)"

.c.o:
	$(CC) $(GGCFLAGS) -c $<

$(OBJ): $(HEAD) config.mk

gridguess: $(OBJ)
	$(CC) -o $@ $(OBJ) $(GGLDFLAGS)

clean:
	rm -f gridguess $(OBJ) gridguess-$(VERSION).tar.gz

dist: clean
	mkdir -p gridguess-$(VERSION)
	cp -R LICENCE Makefile README config.mk \
		$(SRC) $(HEAD) gridguess-$(VERSION)
	tar -cf - gridguess-$(VERSION) | gzip > gridguess-$(VERSION).tar.gz
	rm -rf gridguess-$(VERSION)

install: gridguess
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f gridguess $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/gridguess

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/gridguess

.PHONY: all options clean dist install uninstall
