CC=gcc
CFLAGS=`pkg-config --cflags gtk+-3.0`
LIBS=`pkg-config --libs gtk+-3.0` -lm

all: sglauncher sglauncher-config

sglauncher: sglauncher.c
	$(CC) $(CFLAGS) -o sglauncher sglauncher.c $(LIBS)

sglauncher-config: sglauncher-config.c
	$(CC) $(CFLAGS) -o sglauncher-config sglauncher-config.c $(LIBS)

clean:
	rm -f sglauncher sglauncher-config

