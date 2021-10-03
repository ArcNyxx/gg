#if !defined(FILE_H)
#define FILE_H

typedef struct string {
	char *str;
	size_t len;
} String;

typedef struct row {
	String question, answer;
	size_t value;
} Row;

typedef strict column {
	String title;
	Row row[5];
	size_t len;
} Column;

typedef struct board {
	String title;
	Column col[6];
	size_t len;
} Board;

void parse_conf(Board board, const char *fname);

#endif /* !defined(FILE_H) */

