#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "config.h"

#define ERROR(msg) fprintf(stderr, "gg: invalid syntax: " msg " (%ld)\n", line)
#define EXIT(msg) ERROR(msg); exit(1);

Board
parse_conf(const char *fname)
{
	char *dump = NULL, *que, *ans;
	size_t line = 2, dumplen = 0, dumpalloc = 0;
	Board board = { 0 };

	FILE *file;
	if (!(file = fopen(fname, "r"))) {
		fprintf(stderr, "gg: file not found\n");
		exit(1);
	}

	dumplen = getline(&board.title, &dumpalloc, file) - 1;
	board.title = realloc(board.title, dumplen + sizeof(title));
	memcpy(board.title + dumplen, title, sizeof(title));
	if (getline(&dump, &dumpalloc, file) != 1) {
		EXIT("newline expected");
	}

	for (size_t i = 0; ; ++i, ++line) {
		if ((board.col[i].title.len = getline(
			&board.col[i].title.str, &dumplen, file)) == (size_t)-1) {
			board.len = i;
			break;
		} else if (i >= 6) {
			EXIT("end of file expected")
		}
		board.col[i].title.str = realloc(board.col[i].title.str,
			board.col[i].title.len--);
		board.col[i].title.str[board.col[i].title.len] = 0;

		for (size_t j = 0; ; ++j, ++line) {
			dumplen = getline(&dump, &dumpalloc, file);
			if (dumplen == (size_t)-1) {
				EXIT("row or newline expected")
			} else if (dumplen == 1) {
				board.col[i].len = j;
				break;
			} else if (j >= 5) {
				EXIT("newline expected")
			}

			if (!((que = strstr(dump, delim)) &&
				(ans = strstr(que + 4, delim)))) {
				EXIT("delimeters not found")
			}

			board.col[i].row[j].value.str = malloc(
				(board.col[i].row[j].value.len = que - dump) + 1);
			board.col[i].row[j].question.str = malloc(
				(board.col[i].row[j].question.len = ans - que - 4) + 1);
			board.col[i].row[j].answer.str = malloc(
				(board.col[i].row[j].answer.len =
				dump + dumplen - ans - 4) + 1);

			memcpy(board.col[i].row[j].value.str, dump,
				board.col[i].row[j].value.len + 1);
			memcpy(board.col[i].row[j].question.str, que + 4,
				board.col[i].row[j].question.len + 1);
			memcpy(board.col[i].row[j].answer.str, ans + 4,
				board.col[i].row[j].answer.len + 1);
		}
	}

	free(dump);
	fclose(file);
	return board;
}

