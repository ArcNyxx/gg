#pragma once

#include <exception>
#include <string>

namespace ex
{
	class exit : public std::exception
	{
	public:
		int code;
		std::string msg;

		exit(int code, std::string msg) : code(code), msg(msg) {}
	};
}

