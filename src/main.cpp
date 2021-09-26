#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

#include "exception.hpp"
#include "file.h"
#include "test.hpp"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "gg: board file must be supplied\n";
		return 4;
	}
	
	std::string file = argv[1];
	gg::board board;
	try
	{
		board = gg::readfile(file);
	}
	catch (const ex::exit &err)
	{
		std::cerr << "gg: " << err.msg << "\n";
		return err.code;
	}

	test(board);

	int width = 800, height = 600;
	sf::RenderWindow window(sf::VideoMode(width, height), "GridGuess", sf::Style::Default);

	while (window.isOpen())
	{
		char forward = 0;


		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;

				case sf::Event::Resized:
					width = event.size.width, height = event.size.height;
					break;
			
				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::Space)
					{

					}
			}
		}

		window.clear();
		window.display();
	}
}

