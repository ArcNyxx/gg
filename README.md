# GridGuess
GridGuess is a classroom board game designed to be easy to set up and beginner friendly while still being versatile.
It functions on a grid of unknown questions with values displayed in some columns, with each slot defined in a custom markup language.

## Command Line
GridGuess is invoked from the command line to tell it which board to play.
Type `gg [file]` to play GridGuess! The config file will be judged at runtime for syntax errors.

## Markup Syntax
The first line indicates the title of the grid and is followed by 1-6 columns.
Each column is separated by 2 newlines and starts with the title. Each has 5 slots for questions.
Each slot has the following syntax: `value %% question %% answer`.
Note that because the syntax delimiter is " %% ", this string cannot be used in any text.

### Example
Board Title

Column Title 1
200 %% Question 1-1 %% Answer 1-1
400 %% Question 1-2 %% Answer 1-2
600 %% Question 1-3 %% Answer 1-3

Column Title 2
600 %% Question 2-1 %% Answer 2-1

Column Title 3
100 %% Question 3-1 %% Answer 3-1
200 %% Question 3-2 %% Answer 3-2
300 %% Question 3-3 %% Answer 3-3
400 %% Question 3-4 %% Answer 3-4
500 %% Question 3-5 %% Answer 3-5

Column Title 4
400 %% Question 4-1 %% Answer 4-1
800 %% Question 4-1 %% Answer 4-2

