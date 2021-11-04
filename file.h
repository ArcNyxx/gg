/*
 * statb - simple info bar
 * Copyright (C) 2021 FearlessDoggo21
 * see LICENCE file for licensing information
 */

#ifndef FILE_H
#define FILE_H

#include <stddef.h>
#include <stdio.h>

typedef struct string {
	char *str;
	size_t len;
} String;

typedef struct row {
	char *value, *question, *answer;
} Row;

typedef struct column {
	char *title;
	Row row[5];
	size_t len;
} Column;

typedef struct board {
	char *title;
	Column col[6];
	size_t len;
} Board;

void mkboard(Board *board, FILE *file);

#endif /* FILE_H */

