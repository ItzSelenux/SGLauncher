DATE := $(shell git log -n 1 --format="%ad" --date=format-local:'%Y%m%d/%H:%M:%S')
CFLAGS = `pkg-config --cflags gtk+-3.0` -Dmver=\"$(DATE)\" -Wall
CC = cc
LIBS = `pkg-config --libs gtk+-3.0`
SRC = sglauncher.c sglauncher-cfg.c
OBJ = $(SRC:.c=.o)
EXE = sglauncher

PREFIX = /usr
BIN_DIR = $(PREFIX)/bin
APP_DIR = $(PREFIX)/share/applications

all: $(EXE)
sglauncher: sglauncher.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS) -lm
sglauncher-cfg: sglauncher-cfg.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS) -lm
debug: sglauncher.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS) -lm -g
test:
	./$(EXE)
install:
	install -Dm755 $(EXE) $(BIN_DIR)/$(EXE)
	install -Dm644 $(EXE).desktop $(APP_DIR)/$(EXE).desktop
	install -Dm644 icons/sglauncher.svg $(PREFIX)/share/icons/hicolor/48x48/apps/sglauncher.svg
uninstall:
	rm $(BIN_DIR)/$(EXE)
	rm $(APP_DIR)/$(EXE).desktop
	rm $(PREFIX)/share/icons/hicolor/64x64/apps/menulibre.png
clean:
	rm -f $(OBJ) $(EXE) debug
