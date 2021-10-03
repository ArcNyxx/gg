#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

#include "exception.hpp"
#include "file.h"
#include "test.hpp"

struct status {
	int width, height, x, y, col, row;
};

int main(int argc, char *argv[]) {
	if (argc != 2) {
		std::cerr << "gg: board file must be supplied\n";
		return 4;
	}

	std::string file = argv[1];
	gg::board board;
	try {
		board = gg::readfile(file);
	} catch (const ex::exit &err) {
		std::cerr << "gg: " << err.msg << "\n";
		return err.code;
	}

	status st = { 800, 600, 0, 0, -1, -1 };
	sf::RenderWindow window(sf::VideoMode(st.width, st.height), board.title + " - GridGuess", sf::Style::Default);
	window.setVerticalSyncEnabled(true);
	sf::Font font;
	if (!font.loadFromFile("font/roboto.ttf")) {
		std::cerr << "gg: unable to load font\n";
		return 4;
	}

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::Resized:
				st.width = event.size.width, st.height = event.size.height;
				break;
			
			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Left) {
					st.x = event.mouseButton.x, st.y = event.mouseButton.y;
				}
				break;
			}
		}

		window.clear();

		sf::Text text;
		text.setFont(font);
		text.setPosition(st.width / 2, 200);
		text.setString("Hello World!");
		text.setCharacterSize(24);
		window.draw(text);

		sf::Text text2;
		text2.setFont(font);
		text2.setPosition(st.width / 2, 400);
		text2.setString("This is SFML.");
		text2.setCharacterSize(24);
		window.draw(text2);

		window.display();
	}
}

