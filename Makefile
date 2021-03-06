CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-lwiringPi
SOURCES=main.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=piMessage

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm $(OBJECTS) $(EXECUTABLE)