# gridguess - classroom board game
# Copyright (C) 2021 FearlessDoggo21
# see LICENCE file for licensing information

.POSIX:

include config.mk

SRC = file.c gg.c util.c
HEAD = file.h util.h
OBJ = $(SRC:.c=.o)

all: gg

$(OBJ): $(HEAD) config.mk

.c.o:
	$(CC) $(CFLAGS) -c $<

gg: $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

clean:
	rm -rf gg $(OBJ) gg-$(VERSION).tar.gz

dist: clean
	mkdir -p gg-$(VERSION)
	cp -r LICENCE README Makefile config.mk gg.1 $(SRC) $(HEAD) gg-$(VERSION)
	tar -cf gg-$(VERSION).tar gg-$(VERSION)
	gzip gg-$(VERSION).tar
	rm -rf gg-$(VERSION)

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin $(DESTDIR)$(MANPREFIX)/man1
	cp -f gg $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/gg
	sed "s/VERSION/$(VERSION)/g" < gg.1 > $(DESTDIR)$(MANPREFIX)/man1/gg.1
	chmod 644 $(DESTDIR)$(MANPREFIX)/man1/gg.1

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/gg $(DESTDIR)$(MANPREFIX)/man1/gg.1

options:
	@echo gg build options
	@echo "CFLAGS = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"

# implementation defined
.PHONY: all clean dist install uninstall options
