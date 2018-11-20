CC = gcc
CFLAGS = -O2 -Wall
LFLAGS =

DEP = lcd.h

all:	display_temp

###

display_temp:	display_temp.o
	$(CC) $(LFLAGS) -o display_temp display_temp.o

# changed slightly to include lcd .h file
display_temp.o:	display_temp.c $(DEP)
	$(CC) $(CFLAGS) -c display_temp.c -include $(DEP) 


submit:	display_temp.c README Makefile
	tar -czvf hw9_submit.tar.gz display_temp.c fakedate README Makefile

clean:	
	rm -f *~ *.o display_temp display_final display_cool

