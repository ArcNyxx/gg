gg: src/*.c src/*.h
	cc -Wall -Wextra -O2 src/*.c -o gg -lcsfml-graphics -lcsfml-system -lcsfml-window

all: gg

clean:
	rm -f gg
