/* gridguess - classroom board game
 * Copyright (C) 2021 FearlessDoggo21
 * see LICENCE file for licensing information */

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
main(int argc, const char **argv)
{
	FILE *file;
	Board board = { 0 };

	if (argc != 2)
		die("usage: gg [file]\n");
	if ((file = fopen(argv[1], "r")) == NULL)
		die("gg: file not found\n");
	mkboard(&board, file);
	fclose(file);

	sfFont *font;
	sfText *text = sfText_create();
	if ((font = sfFont_createFromFile("font.ttf")) == NULL)
		die("gg: unable to load font\n");
	sfText_setFont(text, font);

	sfRenderWindow *window;
	if ((window = sfRenderWindow_create((sfVideoMode){ 800, 450, 32 },
		board.title, sfDefaultStyle, NULL)) == NULL)
		die("gg: unable to create window\n");

	struct sigaction act = { 0 };
	act.sa_handler = &handle;
	if (sigaction(SIGINT, &act, NULL) == -1)
		die("gg: unable to set signal handler\n");

	Row *row = NULL;
	long width = 800, height = 450;
	char ans = 0;
	while (sfRenderWindow_isOpen(window) && running) {
		sfEvent event;
		while (sfRenderWindow_pollEvent(window, &event)) {
			switch (event.type) {
			default: break;
			case sfEvtClosed:
				sfRenderWindow_close(window);
				return 0;
			case sfEvtResized:
				width = event.size.width, height = event.size.height;
				/* ensures text is properly aligned on screen no matter size */
				sfRenderWindow_setView(window,
					sfView_createFromRect((sfFloatRect){ 0, 0, width, height }));
				break;
			case sfEvtMouseButtonPressed:
				/* if row is null, set row, ans is ~0
				 * else, bitwise not ans and bitwise and with ptr to zero */
				if (row == NULL) {
					int x = event.mouseButton.x / (width / 6),
						y = event.mouseButton.y / (height / 6) - 1;
					if (x < board.len && y != -1 && y  < board.col[x].len)
						row = &board.col[x].row[y];
				} else {
					ans = !ans;
					if (!ans)
						row = NULL;
				}
			}
		}

		sfRenderWindow_clear(window, sfBlack);
		if (row == NULL) {
			sfVector2f pos = { width / 12, height / 12 };
			for (int i = 0; i < board.len; ++i) {
				sfText_setCharacterSize(text, width / 40);
				sfText_setPosition(text, pos);
				sfText_setString(text, board.col[i].title);

				sfFloatRect bounds = sfText_getLocalBounds(text);
				sfText_setOrigin(text,
					(sfVector2f){ bounds.width / 2, bounds.height / 2 });
				sfRenderWindow_drawText(window, text, NULL);
				for (int j = 0; j < board.col[i].len; ++j) {
					pos.y += height / 6;

					sfText_setCharacterSize(text, width / 25);
					sfText_setPosition(text, pos);
					sfText_setString(text, board.col[i].row[j].value);

					bounds = sfText_getLocalBounds(text);
					sfText_setOrigin(text,
						(sfVector2f){ bounds.width / 2, bounds.height });
					sfRenderWindow_drawText(window, text, NULL);
				}
				pos.x += width / 6, pos.y = height / 12;
			}
		} else {
			sfText_setCharacterSize(text, width / 15);
			sfText_setPosition(text, (sfVector2f){ width / 2, height / 2 });
			sfText_setString(text, ans ? row->answer : row->question);

			sfFloatRect bounds = sfText_getLocalBounds(text);
			sfText_setOrigin(text,
				(sfVector2f){ bounds.width / 2, bounds.height / 2 });
			sfRenderWindow_drawText(window, text, NULL);
		}
		sfRenderWindow_display(window);
	}
}
