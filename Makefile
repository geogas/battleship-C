CC=gcc
CFLAGS=-g3 -Wfatal-errors -Wpointer-arith -c
SOURCES=grid_lib.c util.c user_lib.c rival_lib.c battleship.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=battleship

all: $(SOURCES) $(EXECUTABLE)
	    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ -lcurses

.c.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: all clean

clean :
	rm -f *o battleship
