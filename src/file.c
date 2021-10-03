#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "config.h"

#define ERROR(msg) fprintf(stderr, "gg: invalid syntax: " msg " (%d)\n", line)
#define EXIT(msg) ERROR(msg); exit(1);

Board
parse_conf(const char *fname)
{
	char *dump = NULL;
	size_t line = 2, dumplen = 0;
	Board board = { 0 };

	FILE *file;
	if (!(file = fopen(fname, "r"))) {
		fprintf(stderr, "gg: file not found\n");
		exit(1);
	}

	board.title.len = getline(&board.title.str, &dumplen, file) - 1;
	if (getline(&dump, &dumplen, file) != 1) {
		fprintf(stderr, "gg: invalid syntax: expected blank line (2)\n");
		exit(1);
	}

	for (char i = 0; i < 6; ++i) {
		++line;
		if ((board.col[i].title.len = 
			getline(&board.col[i].title.str, &dumplen, file)) == -1) {
			board.len = i;
			break;
		}

		for (char j = 0; ; ++j) {
			++line;
			size_t temp;
			if ((temp = getline(&dump, &dumplen, file)) == -1) {
				EXIT("row or newline expected")
			} else if (temp == 1) {
				board.col[i].len = j;
				break;
			} else if (j == 6) {
				EXIT("newline expected")
			}

			char *start, *end;
			if (!((start = strstr(dump, delim)) &&
				(end = strstr(start += 4, delim)))) {
				EXIT("delimeters not found")	
			}
			if ((board.col[i].row[j].value = strtoll(dump, NULL, 0)) == 0) {
				EXIT("number >0 expected")
			}

			board.col[i].row[j].question.len = end - start;
			board.col[i].row[j].question.str =
				malloc(board.col[i].row[j].question.len + 1);
			memcpy(board.col[i].row[j].question.str, start,
				board.col[i].row[j].question.len + 1);

			board.col[i].row[j].answer.len = strlen(end + 4) - 1;
			board.col[i].row[j].answer.str =
				malloc(board.col[i].row[j].answer.len + 1);
			strncpy(board.col[i].row[j].answer.str, end + 4,
				board.col[i].row[j].answer.len);
		}
	}

	free(dump);
	fclose(file);
	return board;
}

