#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SFML/Graphics.h>

#include "file.h"
#include "config.h"

static sfText *text;
static volatile sig_atomic_t term = 0;

static void pos_text(sfVector2f pos, const char *text, size_t size_div);
static void handle(int signal);

static void
pos_text(sfVector2f pos, const char *str, size_t size)
{
	sfText_setString(text, str);
	sfText_setCharacterSize(text, size);
	sfText_setPosition(text, pos);

	sfFloatRect bounds = sfText_getLocalBounds(text);
	sfVector2f origin = { bounds.width / 2.f, bounds.height / 2.f };
	sfText_setOrigin(text, origin);
}

static void
handle(int signal)
{
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

	Board board = parse_conf(argv[1]);
	printf("gg: status: config file formatted correctly\n");

	sfFont *font;
	if (!(font = sfFont_createFromFile(fontpath))) {
		fprintf(stderr, "gg: unable to load font\n");
		exit(1);
	}
	text = sfText_create();
	sfText_setFont(text, font);

	printf("gg: status: creating window\n");
	sfRenderWindow *window;
	size_t width = 800, height = 600;
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
	Row *row = NULL;
	size_t move = 0;
	while (sfRenderWindow_isOpen(window) && !term) {
		sfRenderWindow_clear(window, sfBlack);
		
		if (row == NULL) {
			for (size_t i = 0; i < board.len; ++i) {
				sfVector2f pos = { width / 6 * i + width / 12, height / 12 };
				pos_text(pos, board.col[i].title.str, width / 40);
				sfRenderWindow_drawText(window, text, NULL);
				
				for (size_t j = 0; j < board.col[i].len; ++j) {
					pos.y += height / 6;
					pos_text(pos, board.col[i].row[j].value.str, width / 25);
					sfRenderWindow_drawText(window, text, NULL);
				}
			}
		} else {
			sfVector2f pos = { width / 2, height / 2 };
			pos_text(pos, !move ? row->question.str : row->answer.str, width / 15);
			sfRenderWindow_drawText(window, text, NULL);
		}

		sfRenderWindow_display(window);
		
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
					if (row == NULL) {
						register size_t x, y;
						if ((x = event.mouseButton.x / (width / 6)) < board.len) {
							if ((y = event.mouseButton.y / (height / 6) - 1) < board.col[x].len) {
								row = &board.col[x].row[y];
							}
						}
					} else {
						if (move) {
							move = 0;
							row = NULL;
						} else {
							move = 1;
						}
					}
				}
			default: /* FALLTHROUGH */
				break;
			}
		}
	}

	printf("gg: status: exiting\n");
	exit(0);
}

