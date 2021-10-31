#ifndef FILE_H
#define FILE_H

#ifndef _STDDEF_H
#include <stddef.h>
#endif /* _STDDEF_H */

typedef struct string {
	char *str;
	size_t len;
} String;

typedef struct row {
	String value, question, answer;
} Row;

typedef struct column {
	String title;
	Row row[5];
	size_t len;
} Column;

typedef struct board {
	char *title;
	Column col[6];
	size_t len;
} Board;

Board parse_conf(const char *fname);

#endif /* FILE_H */

