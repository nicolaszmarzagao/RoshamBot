all:
	mkdir -p ./build
	gcc -o ./build/bot.out ./src/main.c -lcurl -ljansson -Wall -Werror -Wextra -Wpedantic

run:
	./build/bot.out
