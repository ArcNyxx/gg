/*
 * statb - simple info bar
 * Copyright (C) 2021 FearlessDoggo21
 * see LICENCE file for licensing information
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SFML/Graphics.h>

#include "file.h"
#include "util.h"

static void handle(int signal);

static volatile sig_atomic_t running = 1;

static void
handle(int signal)
{
	(void)signal;
	running = 0;
}

int
main(int argc, const char *argv[])
{
	FILE *file;
	Board board = { 0 };

	if (argc != 2)
		die("gg: board file must be supplied\n");
	if ((file = fopen(argv[1], "r")) == NULL)
		die("gg: file not found\n");
	mkboard(&board, file);
	fclose(file);

	/* create font before window, more common error */
	sfFont *font;
	sfText *text = sfText_create();
	if ((font = sfFont_createFromFile("roboto.ttf")) == NULL)
		die("gg: unable to load font\n");
	sfText_setFont(text, font);

	sfRenderWindow *window;
	size_t width = 800, height = 600;
	if ((window = sfRenderWindow_create((sfVideoMode){ width, height, 32 },
		board.title, sfDefaultStyle, NULL)) == NULL)
		die("gg: unable to create window\n");

	/* signal handler set after window creation important */
	struct sigaction act = { 0 };
	act.sa_handler = &handle;
	if (sigaction(SIGINT, &act, NULL) == -1)
		die("gg: unable to set signal handler\n");

	Row *row = NULL;
	size_t ques = 0;
	while (sfRenderWindow_isOpen(window) && running) {
		sfEvent event;
		while (sfRenderWindow_pollEvent(window, &event)) {
			switch (event.type) {
			default: break;
			case sfEvtClosed:				
				sfRenderWindow_close(window);
				break;
			case sfEvtResized:
				width = event.size.width;
				height = event.size.height;
				break;
			case sfEvtMouseButtonPressed:
				if (row == NULL) {
					register size_t x = event.mouseButton.x / (width / 6);
					register size_t y = event.mouseButton.y / (height / 6);
					if (x < board.len && y - 1 < board.col[x].len)
						row = &board.col[x].row[y - 1];
				} else {
					ques = ~ques;
					row = (Row *)((size_t)row & ques);
				}
				break;
			}
		}

		sfVector2u sz = sfRenderWindow_getSize(window);
		width = sz.x, height = sz.y;

		sfRenderWindow_setView(window, sfView_createFromRect((sfFloatRect){ 0, 0, width, height }));

#define SET_ORIGIN \
bounds = sfText_getLocalBounds(text); \
sfText_setOrigin(text, (sfVector2f) {\
	bounds.left + bounds.width / 2, \
	bounds.top + bounds.height / 2 \
});

		sfVector2f pos, origin;
		sfFloatRect bounds;
		sfRenderWindow_clear(window, sfBlack);
		if (row != NULL) {
			sfText_setCharacterSize(text, width / 15);
			sfText_setPosition(text, (sfVector2f){ width / 2, height / 2 });
			sfText_setString(text, ques ? row->question : row->answer);

			SET_ORIGIN
			sfRenderWindow_drawText(window, text, NULL);
		} else {
			pos = (sfVector2f){ width / 12, height / 12 };
			for (size_t i = 0; i < board.len; ++i) {
				sfText_setCharacterSize(text, width / 25);
				sfText_setPosition(text, pos);
				sfText_setString(text, board.col[i].title);

				SET_ORIGIN
				sfRenderWindow_drawText(window, text, NULL);

				for (size_t j = 0; j < board.col[i].len; ++j) {
					pos.y += height / 6;
					sfText_setCharacterSize(text, width / 25);
					sfText_setPosition(text, pos);
					sfText_setString(text, board.col[i].row[j].value);

					SET_ORIGIN
					sfRenderWindow_drawText(window, text, NULL);
				}
				pos.x += width / 6, pos.y = height / 12;
			}
		}
		sfRenderWindow_display(window);
	}
}
