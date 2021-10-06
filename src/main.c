#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SFML/Graphics.h>

#include "file.h"
#include "config.h"

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
	if (!(font = sfFont_createFromFile(fontpath))) {
		fprintf(stderr, "gg: unable to load font\n");
		exit(1);
	}

	sfText *sm_text = sfText_create(), *lg_text = sfText_create();
	sfText_setFont(sm_text, font);
	sfText_setFont(lg_text, font);

	printf("gg: status: creating window\n");
	sfRenderWindow *window;
	sfVideoMode mode = { width, height, 32 };

	if (!(window = sfRenderWindow_create(
		mode, board.title, sfDefaultStyle, NULL))) {
		fprintf(stderr, "gg: unable to create window\n");
		exit(1);
	}

	printf("gg: status: entering event loop\n");
	while (sfRenderWindow_isOpen(window)) {
		sfEvent event;
		while (sfRenderWindow_pollEvent(window, &event)) {
			switch (event.type) {
			case sfEvtClosed:
				printf("gg: status: exiting\n");
				sfRenderWindow_close(window);
				return 0;
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
		sfText_setCharacterSize(sm_text, width / 40);
		sfText_setCharacterSize(lg_text, width / 20);

		for (size_t i = 0; i < board.len; ++i) {
			sfVector2f pos = { width / 6 * i + width / 12, height / 12 };
			sfText_setString(sm_text, board.col[i].title.str);
			sfText_setPosition(sm_text, pos);

			sfFloatRect bounds = sfText_getLocalBounds(sm_text);
			sfVector2f origin = { bounds.width / 2.f, bounds.height / 2.f };
			sfText_setOrigin(sm_text, origin);
			sfRenderWindow_drawText(window, sm_text, NULL);
			
			for (size_t j = 0; j < board.col[i].len; ++j) {
				pos.y += height / 6;
				sfText_setString(sm_text, board.col[i].row[j].value.str);
				sfText_setPosition(sm_text, pos);

				bounds = sfText_getLocalBounds(sm_text);
				origin.x = bounds.width / 2.f;
				origin.y = bounds.height / 2.f;
				sfText_setOrigin(sm_text, origin);
				sfRenderWindow_drawText(window, sm_text, NULL);
			}
		}

		sfRenderWindow_display(window);
	}
}

