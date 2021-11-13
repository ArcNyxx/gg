/*
 * gridguess - classroom board game
 * Copyright (C) 2021 FearlessDoggo21
 * see LICENCE file for licensing information
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

void
die(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	exit(1);
}

void
addnl(char *str, int cutoff)
{
	int start = 0;
	for (int i = 0; i < 4; ++i) {
		for (int j = start; j < cutoff; ++j)
			if (str[j] == '\0')
				return;
		for (int j = start + cutoff; ; ++j)
			if (str[j] == ' ') {
				str[j] = '\n';
				start = j;
				break;
			} else if (str[j] == '\0') {
				return;
			}
	}
}
