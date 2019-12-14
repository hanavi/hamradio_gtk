all: logbook

logbook: main.c
	gcc -g `pkg-config --cflags gtk+-3.0` -o logbook main.c `pkg-config --libs gtk+-3.0` -rdynamic -l sqlite3


clean:
	rm *~ logbook


