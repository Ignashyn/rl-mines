
CC ?= cc
CFLAGS = -std=c99 -I raylib/raylib-5.0/include/
LDFLAGS =
TARGET_OS ?= win64

ifeq ($(TARGET_OS),linux)
	LDFLAGS = -L raylib/raylib-5.0/lib/linux-amd64 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
else
	LDFLAGS = -L raylib/raylib-5.0/lib/win64-mingw64 -lraylib -lopengl32 -lgdi32 -lwinmm -std=c99
endif

SRCDIR = src
OBJDIR = obj
BINDIR = bin

SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:src/%.c=obj/%.o)
TARGET = rl-mines

all: $(TARGET)

clean:
	rm -fr $(OBJDIR)
	rm -fr $(BINDIR)

.PHONY: all clean

$(TARGET): $(OBJDIR) $(BINDIR) $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $(BINDIR)/$(TARGET)

$(OBJS): $(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $< $(CFLAGS) $(RAYLIB_H_INCPATH) -o $@

$(OBJDIR):
	mkdir $(OBJDIR)

$(BINDIR):
	mkdir $(BINDIR)
