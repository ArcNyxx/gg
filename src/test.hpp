#include <iostream>

#include "file.h"

void test(gg::board board)
{
	std::cout << board.title << std::endl;
        for (char i = 0; i < board.len; ++i)
        {
                std::cout << board.cols[i].title << std::endl;
                for (char j = 0; j < board.cols[i].len; ++j)
                {
                        std::cout << board.cols[i].rows[j].value
                                << board.cols[i].rows[j].question
                                << board.cols[i].rows[j].answer << std::endl;
                }
        }
}

