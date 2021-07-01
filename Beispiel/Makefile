
CFLAGS=-g -Wall

LDFLAGS=

all: erzeuger verbraucher

erzeuger: erzeuger.c
	$(CC) $(CFLAGS) -o erzeuger erzeuger.c $(LDFLAGS) 

verbraucher: verbraucher.c
	$(CC) $(CFLAGS) -o verbraucher verbraucher.c $(LDFLAGS) 

clean: 
	rm erzeuger verbraucher
