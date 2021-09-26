#pragma once

#include <string>

namespace gg
{
	struct row
	{
		std::string question, answer;
		unsigned int value;
	};

	struct column
	{
		std::string title;
		row rows[5];
		unsigned char len = 0;
	};

	struct board
	{
		std::string title;
		column cols[6];
		unsigned char len = 0;
	};

	board readfile(std::string name);
}

