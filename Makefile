



CC = gcc

CFLAGS = `pkg-config --cflags gtk+-3.0`

LIBS = `pkg-config --libs gtk+-3.0` -lm

# File names

SRC = sglauncher.c sglauncher-config.c

OBJ = $(SRC:.c=.o)

EXE = sglauncher sglauncher-config

# Build executable files

all: $(EXE)

sglauncher: sglauncher.o

	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

sglauncher-config: sglauncher-config.o

	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

# Debug step

debug:

	$(CC) $(CFLAGS) -g $(SRC) -o debug $(LIBS)

# Test step

test:

	./sglauncher

	./sglauncher-config

# Clean object files and executables

clean:

	rm -f $(OBJ) $(EXE) debug
