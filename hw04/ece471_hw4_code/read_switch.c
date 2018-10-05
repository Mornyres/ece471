#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>	/* open() */
#include <unistd.h>	/* close() */
#include <string.h>

int main(int argc, char **argv) {

	// enable GPIO17
	int filedesc = open ("/sys/class/gpio/export", O_WRONLY);

	if (filedesc < 0){
		printf("Could not open GPIO export file\r\n");
		exit(1);
	}
	write (filedesc, "17", 2); 
	close (filedesc);

	// set GPIO17 to input
	filedesc = open("/sys/class/gpio/gpio17/direction", O_WRONLY);
	if (filedesc < 0){
		printf("Could not open GPIO direction file\r\n");
		exit(2);
	}
	write (filedesc, "in", 2); 
	close (filedesc);

	// read initial value of GPIO17
	char gpiobuffer[16]; 
	filedesc = open ("/sys/class/gpio/gpio17/value", O_RDONLY);
	if (filedesc < 0){
		printf("Could not open GPIO value file\r\n");
		exit(3);
	}
        read (filedesc, gpiobuffer, 16);
        close (filedesc);

	// in theory error checking isn't too necessary in the main loop because the occasional hiccup may occur and the loop can continue just fine
	while (1) {
	
		while (gpiobuffer[0] == '0') {
			filedesc = open ("/sys/class/gpio/gpio17/value", O_RDONLY);
        		read (filedesc, gpiobuffer, 16);
        		close (filedesc);
		}
		printf("[ PRESS ]\r\n");
		while (gpiobuffer[0] == '1') {
			filedesc = open ("/sys/class/gpio/gpio17/value", O_RDONLY);
        		read (filedesc, gpiobuffer, 16);
        		close (filedesc);
		}
		printf("[ RELEASE ]\r\n"); 
	
	}

	return 0;
}

