CC=gcc
CFLAGS=-Wall
EXEC =ent1 ent
HEADERS= $(wildcard *.h)
OBJECTS = entity.o tools.o  
all: $(EXEC)



ent1 : $(OBJECTS) ent1.o

ent : $(OBJECTS) ent.o

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o
	rm -f *~
proper:	
	rm -f $(EXEC)
