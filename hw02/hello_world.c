/*
 *	ECE 477
 *
 *	HW 02
 *	Forest LeBlanc
 *
 *
 */
#include <stdint.h>
#include <stdio.h>

int main(int argc, char **argv) {

	uint8_t line;
	uint8_t maxline = 15;

	for (line=0; line<maxline;	line++){
		printf("#%d: ECE 471 HELLO\r\n",line);
	}
	return 0;
}
