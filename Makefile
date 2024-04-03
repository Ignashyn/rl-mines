
CC      = gcc
CFLAGS  = -std=c99 -I raylib/raylib-5.0/include
LDFLAGS = -L raylib/raylib-5.0/lib
LDLIBS  = -lraylib

TARGET = rl-mines

ifeq ($(OS),Windows_NT)
	TARGET  := $(TARGET).exe
	LDLIBS  := $(LDLIBS) -lopengl32 -lgdi32 -lwinmm

	ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
		CCFLAGS := $(CFLAGS) -mwindows -m64
		LDFLAGS := $(LDFLAGS)/win64_mingw-w64
	endif
else
	UNAME = $(shell uname)

	ifeq ($(UNAME),Linux)
		LDFLAGS := $(LDFLAGS)/linux_amd64
		LDLIBS  := $(LDLIBS) -lm
	endif
endif

.PHONY: all clean

all: $(TARGET)

clean:
	@rm $(TARGET)

$(TARGET):
	$(CC) mines.c $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o $(TARGET)
