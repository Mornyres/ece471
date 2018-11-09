CC = gcc
CFLAGS = -g -O2 -Wall
LFLAGS =


all:	temp_1wire temp_cool

###

temp_cool:	temp_cool.o
	$(CC) $(LFLAGS) -o temp_cool temp_cool.o

temp_cool.o:	temp_cool.c sensor_name.h
	$(CC) $(CFLAGS) -c temp_cool.c

###

temp_1wire:	temp_1wire.o
	$(CC) $(LFLAGS) -o temp_1wire temp_1wire.o

temp_1wire_c:	temp_1wire.c
	$(CC) $(CFLAGS) -c temp_1wire.c


submit:	temp_1wire.c README Makefile temp_cool.c sensor_name.h sample.sh
	tar -czvf hw8_submit.tar.gz Makefile temp_1wire.c temp_cool.c sensor_name.h README sample.sh

clean:	
	rm -f *~ *.o temp_1wire temp_cool

