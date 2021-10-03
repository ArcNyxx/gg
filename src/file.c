#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"

#define ERROR(msg) fprintf(stderr, "gg: invalid syntax: " msg " (%d)\n", line)
#define EXIT(msg) ERROR(msg); exit(1);

#define STRCPY(dest, src, len) \
	dest = malloc(len + 1); \
	memcpy(dest, src, len + 1);

void
parse_conf(Board board, const char *fname)
{
	char *dump;
	unsigned int line = 3, dumplen = 0;

	FILE *file
	if (!(file = fopen(fname, "r"))) {
		fprintf(stderr, "gg: file not found\n");
		exit(1);
	}

	getline(&board.title.str, &board.title.len, file);
	if (getline(NULL, NULL, file) != 1) {
		fprintf(stderr, "gg: invalid syntax: expected blank line (2)\n");
		exit(1);
	}

	for (char i = 0; i < 6; ++i) {
		++line;
		if (getline(&board.col[i].title.str, &board.col[i].title.len, file) == -1) {
			board.len = i;
			break;
		}

		for (char j = 0; ; ++j) {
			++line;
			if (getline(&dump, &dumplen, file) == -1) {
				EXIT("row or newline expected")
			} else if (dumplen == 1) {
				board.col[i].len = j;
				break;
			} else if (j == 6) {
				EXIT("newline expected")
			}

			const char *delim = " %% ";
			int start = strstr(dump, delim) + 4;
			int end = strstr(dump + start, delim);
			
			if (!(start && end)) {
				EXIT("delimeters not found")
			}
			if ((board.col[i].row[j].value = strtoll(dump, NULL, 0)) == 0) {
				EXIT("number >0 expected")
			}
			
			board.col[i].row[j].question.len = end - start;
			STRCPY(board.col[i].row[j].question.str, start,
				board.col[i].row[j].question.len)
			board.col[i].row[j].answer.len = strlen(end + 4);
			STRCPY(board.col[i].row[j].answer.str, start,
				board.col[i].row[j].answer.len)

			free(dump);
			dumplen = 0;
		}
	}

	free(dump);
	fclose(file);
}

