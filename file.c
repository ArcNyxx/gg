/* gridguess - classroom board game
 * Copyright (C) 2021-2022 FearlessDoggo21
 * see LICENCE file for licensing information */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "util.h"

static void strwrap(char *str, int cutoff);

static void
strwrap(char *str, int cutoff)
{
	size_t start = 0;
	for (size_t i = 0; ; ++i) {
		size_t endl = start + cutoff;
		for (size_t j = start; j < endl; ++j)
			if (str[j] == '\0')
				return;
		for (size_t j = endl; ; ++j)
			if (str[j] == ' ') {
				str[j] = '\n';
				start = j;
				break;
			} else if (str[j] == '\0') {
				return;
			}
	}
}

void
mkboard(Board *board, FILE *file)
{
	char *content = NULL, *que = NULL, *ans = NULL;
	size_t dump = 0, lines = 2, len;

	if ((dump = getline(&board->title, &dump, file)) == (size_t)-1)
		die("gg: invalid syntax: title expected (1)\n");
	board->title[dump - 1] = '\0';
	if (getline(&content, &dump, file) != 1)
		die("gg: invalid syntax: newline expected (2)\n");

	for (size_t i = 0; ; ++i, ++lines) {
		if ((len = getline(&board->col[i].title, &dump, file)) == (size_t)-1) {
			free(content);
			board->len = i;
			break;
		} else if (i >= 6) {
			die("gg: invalid syntax: end of file expected (%ld)\n", lines);
		}
		board->col[i].title[len] = '\0'; /* just overwrites '\n' */
		strwrap(board->col[i].title, 11);

		for (size_t j = 0; ; ++j, ++lines) {
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

			/* no reallocating for speed, just terminate strings
			 * as they remain for application lifetime */
			board->col[i].row[j].value = content;
			board->col[i].row[j].question = que + 4;
			board->col[i].row[j].answer = ans + 4;
			*que = '\0', *ans = '\0', *(content + len - 1) = '\0';
			content = NULL;

			strwrap(board->col[i].row[j].question, 23);
			strwrap(board->col[i].row[j].answer, 23);
		}
	}
}
