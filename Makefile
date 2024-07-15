DATE := $(shell git log -n 1 --format="%ad" --date=format-local:'%Y%m%d/%H:%M:%S')
CFLAGS = `pkg-config --cflags gtk+-3.0` -Dmver=\"$(DATE)\"
CC = gcc
LIBS = `pkg-config --libs gtk+-3.0`
SRC = sglauncher.c sglauncher-cfg.c
OBJ = $(SRC:.c=.o)
EXE = sglauncher sglauncher-cfg

all: $(EXE)
sglauncher: sglauncher.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS) -lm
sglauncher-cfg: sglauncher-cfg.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS) -lm
debug: sglauncher.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS) -lm -g
test:
	./sglauncher
clean:
	rm -f $(OBJ) $(EXE) debug
