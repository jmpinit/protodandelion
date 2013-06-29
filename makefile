PROG := dandelion

ifeq ($(OS),Windows_NT)
S := \\
else
S := /
endif

SRCDIR := src
OBJDIR := obj
BINDIR := bin

SOURCES =	main.c \
			satellite.c
			
INCLUDES = -Isrc$(S)inc -I/usr/local/include -I/usr/include/lua5.1
OBJECTS = $(patsubst %,$(OBJDIR)$(S)%,$(SOURCES:.c=.o))

CFLAGS := -Wall -pedantic -std=c99 -g -Os -gstabs
LFLAGS = `sdl-config --libs` -lSDL -L/usr/local/lib -llacewing -llua5.1
CC := gcc

all: $(PROG)

run: $(PROG)
	bin$(S)$(PROG)
	
debug: $(PROG)
	gdb bin$(S)$(PROG)

# linking the program.
$(PROG): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(BINDIR)$(S)$(PROG) $(LFLAGS)

# compiling source files.
$(OBJDIR)$(S)%.o: $(SRCDIR)$(S)%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -s -o $@ $<

clean:
ifeq ($(OS),Windows_NT)
	del $(OBJECTS)
else
	rm $(OBJECTS)
endif

.PHONY: all clean

