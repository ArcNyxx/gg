/* gridguess - classroom board game
 * Copyright (C) 2021 FearlessDoggo21
 * see LICENCE file for licensing information */

typedef struct string {
	char *str;
	int len;
} String;

typedef struct row {
	char *value, *question, *answer;
} Row;

typedef struct column {
	char *title;
	Row row[5];
	int len;
} Column;

typedef struct board {
	char *title;
	Column col[6];
	int len;
} Board;

void mkboard(Board *board, FILE *file);
