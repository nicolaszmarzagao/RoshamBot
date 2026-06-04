all:
	mkdir -p ./build
	gcc -o ./build/bot.out ./src/main.c ./src/roshambo.c ./src/roshambo.h \
	-lcurl -ljansson -Wall -Werror -Wextra -Wpedantic

run:
	./build/bot.out
