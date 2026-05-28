all:
	gcc -o ./build/bot.out main.c -lcurl -ljansson

run:
	./build/bot.out
