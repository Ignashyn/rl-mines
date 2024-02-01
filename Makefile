
CC ?= gcc
CCFLAGS = -std=c99
INCLUDE = -I raylib/raylib-5.0/include
LDFLAGS = -L raylib/raylib-5.0/lib
LDLIBS = -lraylib

TARGET = rl-mines
SRCDIR = src
OBJDIR = obj
BINDIR = bin

SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:src/%.c=obj/%.o)

ifeq ($(OS),Windows_NT)
TARGET := $(TARGET).exe
LDFLAGS := $(LDFLAGS)/win64_mingw-w64
LDLIBS += -lopengl32 -lgdi32 -lwinmm

RMDIR = rmdir /s /q
else
UNAME = $(shell uname)

ifeq ($(UNAME),Linux)
LDFLAGS := $(LDFLAGS)/linux_amd64
LDLIBS += -lm
RMDIR = rm -fr
endif

endif

.PHONY: all clean

all: $(TARGET)

clean:
	$(RMDIR) $(OBJDIR)
	$(RMDIR) $(BINDIR)

$(TARGET): $(OBJDIR) $(BINDIR) $(OBJS)
	$(CC) -O3 $(OBJS) $(CCFLAGS) $(LDFLAGS) $(LDLIBS) -o $(BINDIR)/$(TARGET)

$(OBJS): $(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $< $(CCFLAGS) $(INCLUDE) -o $@

$(OBJDIR):
	mkdir $(OBJDIR)

$(BINDIR):
	mkdir $(BINDIR)
