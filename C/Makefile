CC=gcc -pthread
CFLAGS=-Wall 
EXEC =ent1 ent
HEADERS= $(wildcard *.h)
OBJECTS = udp.o entity.o tools.o  
all: $(EXEC)


ent2 : $(OBJECTS) ent2.o

ent1 : $(OBJECTS) ent1.o

ent : $(OBJECTS) ent.o

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o
	rm -f *~	
	rm -f $(EXEC)
