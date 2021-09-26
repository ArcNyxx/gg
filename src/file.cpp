#include <fstream>
#include <string>
#include <vector>

#include "exception.hpp"
#include "file.h"

gg::board gg::readfile(std::string name) {
	unsigned int line = 3;
	gg::board board;
	std::string dump;
	std::ifstream file(name);
	if (!file.is_open()) {
		throw ex::exit(4, "file not found");
	}

	std::getline(file, board.title);
	std::getline(file, dump);
	if (file.eof()) {
		throw ex::exit(2, "invalid syntax: no board info");
	}

	for (char i = 0; i < 6; ++i) {
		++line;
		std::getline(file, board.cols[i].title);
		if (file.eof()) {
			board.len = i;
			break;
		}

		for (char j = 0; ; ++j) {
			++line;
			getline(file, dump);
			if (file.eof()) {
				throw ex::exit(2, "invalid syntax: row or newline expected (" + std::to_string(line) + ")");
			} else if (dump == "") {
				board.cols[i].len = j;
				break;
			} else if (j == 6) {
				throw ex::exit(2, "invalid syntax: newline expected (" + std::to_string(line) + ")");
			}

			int start = dump.find(" %% ") + 4, end = dump.find(" %% ", start);
			if (start == std::string::npos || end == std::string::npos) {
				throw ex::exit(2, "invalid syntax: separator tokens not found (" + std::to_string(line) + ")");
			}

			try {
				board.cols[i].rows[j].value = std::stoi(dump);
			} catch (std::invalid_argument &err) {
				throw ex::exit(2, "invalid syntax: number expected (" + std::to_string(line) + ")");
			}
			board.cols[i].rows[j].question = dump.substr(start, end - start);
			board.cols[i].rows[j].answer = dump.substr(end + 4);
		}
	}

	file.close();
	return board;
}

