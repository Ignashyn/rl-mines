
CC = gcc

all:
	$(CC) rl-mines.c -std=c99 -I raylib/raylib-5.0/include -mwindows -m64 -L raylib/raylib-5.0/lib/win64_mingw-w64 -lraylib -lopengl32 -lgdi32 -lwinmm -o rl-mines.exe
