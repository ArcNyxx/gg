#ifndef FILE_H
#define FILE_H

#ifndef _STDDEF_H
#include <stddef.h>
#endif /* _STDDEF_H */
#ifndef _STDIO_H
#include <stdio.h>
#endif /* _STDIO_H */

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

