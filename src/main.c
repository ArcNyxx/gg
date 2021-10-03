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

	int width = 800, height = 600, x, y, col, row, que = 1;
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
	sfText_setCharacterSize(sm_text, 24);
	sfText_setCharacterSize(lg_text, 44);

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
				sfRenderWindow_close(window);
				break;
			case sfEvtResized:
				width = event.size.width, height = event.size.width;
				break;
			case sfEvtMouseButtonPressed:
				if (event.mouseButton.button == sfMouseLeft) {
					x = event.mouseButton.x, y = event.mouseButton.y;
				}
				break;
			}
		}
		
		sfRenderWindow_clear(window, sfBlack);

		int rows = width / 6, cols = height / 6;
		for (int i = 0; i < board.len; ++i) {
			sfVector2f vec = { rows * i + 5, cols / 2 };
			sfText_setPosition(sm_text, vec);
			sfText_setString(sm_text, board.col[i].title.str);
			sfRenderWindow_drawText(window, sm_text, NULL);

			for (int j = 0; j < board.col[i].len; ++j) {
				sfVector2f vecrow = { rows * i + 5, cols * i + cols / 2 };
				sfText_setPosition(sm_text, vec);
				sfText_setString(sm_text, board.col[i].row[i].value.str);
				sfRenderWindow_drawText(window, sm_text, NULL);
			}
		}

		sfRenderWindow_display(window);
	}
}

