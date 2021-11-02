# gridguess - classroom board game
# Copyright (C) 2021 FearlessDoggo21
# see LICENCE file for licensing information

# gridguess version
VERSION = 0.0.0-indev

# paths
PREFIX = /usr/local

# includes and libs
LIBS = -lcsfml-graphics -lcsfml-system -lcsfml-window

# flags
WPROFILE = -Wall -Wextra -Wmissing-prototypes -Wstrict-prototypes \
-Wmissing-declarations -Wswitch-default -Wunreachable-code -Wcast-align \
-Wpointer-arith -Wbad-function-cast -Winline -Wundef -Wnested-externs \
-Wcast-qual -Wshadow -Wwrite-strings -Wno-unused-parameter -Wfloat-equal
GGCPPFLAGS = -DVERSION=\"$(VERSION)\" -D_XOPEN_SOURCE=600 $(WPROFILE) \
	-std=c99 -pedantic -g
GGCFLAGS = $(GGCPPFLAGS) $(CPPFLAGS) $(CFLAGS)
GGLDFLAGS = $(LIBS) $(LDFLAGS)
