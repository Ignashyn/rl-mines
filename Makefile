
CC ?= cc
CFLAGS = -std=c99 -I raylib/raylib-5.0/include/
TARGET_OS ?= win64
LDFLAGS = -L raylib/raylib-5.0/lib/$(TARGET_OS)
LDLIBS = -lraylib

ifeq ($(TARGET_OS),linux-amd64)
	LDLIBS += -lGL -lm -lpthread -ldl -lrt -lX11
else
	LDLIBS += -lopengl32 -lgdi32 -lwinmm -std=c99
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
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o $(BINDIR)/$(TARGET)

$(OBJS): $(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $< $(CFLAGS) -o $@

$(OBJDIR):
	mkdir $(OBJDIR)

$(BINDIR):
	mkdir $(BINDIR)
