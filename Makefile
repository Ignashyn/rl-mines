
CC      = gcc
CCFLAGS = -std=c99
INCLUDE = -I raylib/raylib-5.0/include
LDFLAGS = -L raylib/raylib-5.0/lib
LDLIBS  = -lraylib

TARGET = rl-mines
SRCDIR = src
OBJDIR = obj
BINDIR = bin

SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:src/%.c=obj/%.o)

ifeq ($(OS),Windows_NT)
	RMDIR   = rd /s /q

	TARGET  := $(TARGET).exe
	LDLIBS  := $(LDLIBS) -lopengl32 -lgdi32 -lwinmm

	ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
		CCFLAGS := $(CCFLAGS) -mwindows -m64
		LDFLAGS := $(LDFLAGS)/win64_mingw-w64
	endif
	ifeq ($(PROCESSOR_ARCHITECTURE),x86)
		CCFLAGS := $(CCFLAGS) -mwindows -m32
		LDFLAGS := $(LDFLAGS)/win32_mingw-w64
	endif
else
	UNAME = $(shell uname)

	ifeq ($(UNAME),Linux)
		LDFLAGS := $(LDFLAGS)/linux_amd64
		LDLIBS  := $(LDLIBS) -lm
		RMDIR   = rm -fr
	else
		@echo "Unidentified OS. No flags set."
	endif
endif

.PHONY: all clean

all: $(TARGET)

clean:
	$(RMDIR) $(OBJDIR)
	$(RMDIR) $(BINDIR)

$(TARGET): $(OBJDIR) $(BINDIR) $(OBJS)
	$(CC) -O2 $(OBJS) $(CCFLAGS) $(LDFLAGS) $(LDLIBS) -o $(BINDIR)/$(TARGET)

$(OBJS): $(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $< $(CCFLAGS) $(INCLUDE) -o $@

$(OBJDIR):
	mkdir $(OBJDIR)

$(BINDIR):
	mkdir $(BINDIR)
