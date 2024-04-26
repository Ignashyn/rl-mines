
CC = gcc

rl-mines:
	$(CC) rl-mines.c -std=c99 -I raylib/raylib-5.0/include -L raylib/raylib-5.0/lib/linux_amd64 -lraylib -lm -o rl-mines
