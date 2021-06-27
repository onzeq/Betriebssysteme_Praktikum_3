CFLAGS=-g -Wall

LDFLAGS=

all: application spooler printer_1 printer_2

application: application.c
	$(CC) $(CFLAGS) -o application application.c $(LDFLAGS) 

spooler: spooler.c
	$(CC) $(CFLAGS) -o spooler spooler.c $(LDFLAGS)

printer_1: printer_1.c
	$(CC) $(CFLAGS) -o printer_1 printer_1.c $(LDFLAGS)

printer_2: printer_2.c
	$(CC) $(CFLAGS) -o printer_2 printer_2.c $(LDFLAGS)

clean: 
	rm application spooler printer_1 printer_2
