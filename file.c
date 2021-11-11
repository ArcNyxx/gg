/*
 * gridguess - classroom board game
 * Copyright (C) 2021 FearlessDoggo21
 * see LICENCE file for licensing information
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "util.h"

void
mkboard(Board *board, FILE *file)
{
	char *content = NULL, *que = NULL, *ans = NULL;
	unsigned long dump, lines = 2, len;

	(void)getline(&board->title, &dump, file);
	if (getline(&content, &dump, file) != 1)
		die("gg: invalid syntax: newline expected (2)\n");

	for (int i = 0; ; ++i, ++lines) {
		if ((len = getline(&board->col[i].title, &dump, file)) == (size_t)-1) {
			free(content);
			board->len = i;
			break;
		} else if (i >= 6) {
			die("gg: invalid syntax: end of file expected (%ld)\n", lines);
		}
		board->col[i].title[len] = '\0'; /* just overwrites '\n', leaves extra */;

		for (int j = 0; ; ++j, ++lines) {
			if ((len = getline(&content, &dump, file)) == (size_t)-1) {
				die("gg: invalid syntax: row or newline expected (%ld)\n", lines);
			} else if (len == 1) {
				board->col[i].len = j;
				break;
			} else if (j >= 5) {
				die("gg: invalid syntax: newline expected (%ld)\n", lines);
			}

			if ((que = strstr(content, " %% ")) == NULL ||
				(ans = strstr(que + 4, " %% ")) == NULL)
				die("gg: invalid syntax: delimeters not found (%ld)\n", lines);

			/* 
			 * no reallocating for speed, just terminate strings
			 * they remain for application lifetime, no hanging pointers
			 */
			board->col[i].row[j].value = content;
			board->col[i].row[j].question = que + 4;
			board->col[i].row[j].answer = ans + 4;
			*que = '\0', *ans = '\0', *(content + len - 1) = '\0';
			content = NULL;
		}
	}
}
