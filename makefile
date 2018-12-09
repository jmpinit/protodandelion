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
			sat.c \
			sat_util.c \
			sprite.c \
			list.c \
			satlib$(S)satlib.c \
			satlib$(S)initialize.c \
			satlib$(S)construct.c \
			satlib$(S)utility.c \
			betalib$(S)beta.c \
			termlib$(S)term.c \
			
INCLUDES = -Isrc$(S)inc
OBJECTS = $(patsubst %,$(OBJDIR)$(S)%,$(SOURCES:.c=.o))

CFLAGS := -Wall -pedantic -std=gnu99 -g -O0
LFLAGS = `sdl-config --libs` -llua -lSDL_image
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

