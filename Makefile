



CC = gcc

CFLAGS = `pkg-config --cflags gtk+-3.0`

LIBS = `pkg-config --libs gtk+-3.0` -lm

# File names

SRC = sglauncher.c sglauncher-cfg.c

OBJ = $(SRC:.c=.o)

EXE = sglauncher sglauncher-cfg

# Build executable files

all: $(EXE)

sglauncher: sglauncher.o

	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

sglauncher-cfg: sglauncher-cfg.o

	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

# Debug step

debug:

	$(CC) $(CFLAGS) -g $(SRC) -o debug $(LIBS)

# Test step

test:

	./sglauncher

	./sglauncher-cfg

# Clean object files and executables

clean:

	rm -f $(OBJ) $(EXE) debug
