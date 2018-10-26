CC = gcc
CFLAGS = -g -O2 -Wall
LFLAGS =


all:	i2c-test i2c-cool

###

gpio.o:	gpio.c
	$(CC) $(CFLAGS) -c gpio.c

###

i2c-bitbang.o:	i2c-bitbang.c
	$(CC) $(CFLAGS) -c i2c-bitbang.c

###

i2c-test:	i2c-test.o i2c-bitbang.o gpio.o
	$(CC) $(LFLAGS) -o i2c-test i2c-test.o i2c-bitbang.o gpio.o

i2c-test.o:	i2c-test.c
	$(CC) $(CFLAGS) -c i2c-test.c

###

i2c-cool:	i2c-cool.o i2c-bitbang.o gpio.o
	$(CC) $(LFLAGS) -o i2c-cool i2c-cool.o i2c-bitbang.o gpio.o

i2c-cool.o:	i2c-cool.c
	$(CC) $(CFLAGS) -c i2c-cool.c


submit:	i2c-bitbang.c i2c-test.c i2c-bitbang.h i2c-cool.c README Makefile
	tar -czvf hw6_submit.tar.gz i2c-bitbang.c i2c-bitbang.h i2c-test.c i2c-cool.c README Makefile

clean:	
	rm -f *~ *.o i2c-cool i2c-test

