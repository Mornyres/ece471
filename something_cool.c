/*
 *	ECE 477
 *
 *	HW 02
 *	Forest LeBlanc
 *
 *
 */



#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

#define NUMCOLORS 7

static char* colorcodes[]=
{
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN,
	RESET
};

int main(int argc, char **argv) {

	uint8_t letter, line, msglen;
	uint8_t maxlen=32;
	char* msg="ECE 471 HELLO";
	uint8_t maxline;

	if (argc == 2){
		maxline =  atoi(argv[1]);
		if (maxline == 0){
			printf("Did you mean to do that? Try a number like 7\r\n"); // this is the most error checking i'll bother with, it quickly becomes a wormhole
		}
	}
	else{
		maxline = 15;
		printf("Also try specifying line count, e.g. ./something_cool 7\r\n");
	}

	msglen = snprintf(msg,maxlen,"ECE 471 HELLO"); // format msg buffer 
	for (line=0; line<maxline;	line++){
		printf("#%d:",line);
		for (letter=0; letter<msglen;	letter++){
			printf("%s",colorcodes[rand() % NUMCOLORS]); // select random color code from table above
			printf("%c",msg[letter]);	// print character in message
			printf("%s",RESET);	// reset colorcode
				
		}
		printf("\r\n");
	}
	return 0;
}
