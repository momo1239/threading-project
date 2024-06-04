CC = gcc
CFLAGS = -Wall -pthread
OBJECTS = main.o receiver.o processor.o responder.o

all: program

program: $(OBJECTS)
	$(CC) $(CFLAGS) -o program $(OBJECTS)

main.o: main.c common.h receiver.h processor.h responder.h
	$(CC) $(CFLAGS) -c main.c

receiver.o: receiver.c receiver.h common.h
	$(CC) $(CFLAGS) -c receiver.c

processor.o: processor.c processor.h common.h
	$(CC) $(CFLAGS) -c processor.c

responder.o: responder.c responder.h common.h
	$(CC) $(CFLAGS) -c responder.c

clean:
	rm -f *.o program
