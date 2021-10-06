#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SFML/Graphics.h>

#include "file.h"
#include "config.h"

static volatile sig_atomic_t term = 0;

static void handle(int signal);

static void
handle(int signal) {
	(void)signal;
	term = 1;
}

int
main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "gg: invalid argument: board file must be supplied\n");
		exit(1);
	}

	size_t width = 800, height = 600, x, y, col, row, que = 1;
	Board board = parse_conf(argv[1]);
	printf("gg: status: config file formatted correctly\n");

	sfFont *font;
	sfText *text = sfText_create();
	if (!(font = sfFont_createFromFile(fontpath))) {
		fprintf(stderr, "gg: unable to load font\n");
		exit(1);
	}
	sfText_setFont(text, font);

	printf("gg: status: creating window\n");
	sfRenderWindow *window;
	sfVideoMode mode = { width, height, 32 };
	if (!(window = sfRenderWindow_create(
		mode, board.title, sfDefaultStyle, NULL))) {
		fprintf(stderr, "gg: unable to create window\n");
		exit(1);
	}

	/* signal handler set after window creation important */
	struct sigaction act = { 0 };
	act.sa_handler = &handle;
	if (sigaction(SIGINT, &act, NULL) == -1) {
		fprintf(stderr, "gg: unable to set signal handler\n");
		exit(1);
	}

	printf("gg: status: entering event loop\n");
	while (sfRenderWindow_isOpen(window) && !term) {
		sfEvent event;
		while (sfRenderWindow_pollEvent(window, &event)) {
			switch (event.type) {
			case sfEvtClosed:				
				sfRenderWindow_close(window);
				break;
			case sfEvtResized:
				width = event.size.width;
				height = event.size.height;
				break;
			case sfEvtMouseButtonPressed:
				if (event.mouseButton.button == sfMouseLeft) {
					x = event.mouseButton.x;
					y = event.mouseButton.y;
				}
			default: /* FALLTHROUGH */
				break;
			}
		}

		sfRenderWindow_clear(window, sfBlack);
		for (size_t i = 0; i < board.len; ++i) {
			sfVector2f pos = { width / 6 * i + width / 12, height / 12 };
			sfText_setCharacterSize(text, width / 40);
			sfText_setString(text, board.col[i].title.str);
			sfText_setPosition(text, pos);

			sfFloatRect bounds = sfText_getLocalBounds(text);
			sfVector2f origin = { bounds.width / 2.f, bounds.height / 2.f };
			sfText_setOrigin(text, origin);
			sfRenderWindow_drawText(window, text, NULL);
			
			for (size_t j = 0; j < board.col[i].len; ++j) {
				pos.y += height / 6;
				sfText_setCharacterSize(text, width / 25);
				sfText_setString(text, board.col[i].row[j].value.str);
				sfText_setPosition(text, pos);

				bounds = sfText_getLocalBounds(text);
				origin.x = bounds.width / 2.f;
				origin.y = bounds.height / 2.f;
				sfText_setOrigin(text, origin);
				sfRenderWindow_drawText(window, text, NULL);
			}
		}

		sfRenderWindow_display(window);
	}

	printf("gg: status: exiting\n");
	exit(0);
}

