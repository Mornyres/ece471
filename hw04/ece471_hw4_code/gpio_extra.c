#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>	/* open() */
#include <unistd.h>	/* close() */
#include <string.h>

#define ON 1
#define OFF 0

int main(int argc, char **argv) {

	int ledstate = OFF;

	/* begin GPIO17 init */
	// enable GPIO17
	int filedesc17 = open ("/sys/class/gpio/export", O_WRONLY);

	if (filedesc17 < 0){
		printf("Could not open GPIO export file\r\n");
		exit(1);
	}
	write (filedesc17, "17", 2); 
	close (filedesc17);

	// set GPIO17 to input
	filedesc17 = open("/sys/class/gpio/gpio17/direction", O_WRONLY);
	if (filedesc17 < 0){
		printf("Could not open GPIO direction file\r\n");
		exit(2);
	}
	write (filedesc17, "in", 2); 
	close (filedesc17);

	// read initial value of GPIO17
	char gpiobuffer[16]; 
	filedesc17 = open ("/sys/class/gpio/gpio17/value", O_RDONLY);
	if (filedesc17 < 0){
		printf("Could not open GPIO value file\r\n");
		exit(3);
	}
        read (filedesc17, gpiobuffer, 16);
        close (filedesc17);

	/* end of GPIO17 init */

	/* begin GPIO18 init */

	// enable GPIO18
	int filedesc18 = open ("/sys/class/gpio/export", O_WRONLY);

	if (filedesc18 < 0){
		printf("Could not open GPIO export file\r\n");
		exit(1);
	}

	strncpy(gpiobuffer,"18",2);
	write (filedesc18,gpiobuffer, 2); 
	close (filedesc18);

	// set GPIO18 to output
	filedesc18 = open("/sys/class/gpio/gpio18/direction", O_WRONLY);
	if (filedesc18 < 0){
		printf("Could not open GPIO direction file\r\n");
		exit(2);
	}
	write (filedesc18, "out", 3); 
	close (filedesc18);

	/* end GPIO18 init */


	filedesc18 = open("/sys/class/gpio/gpio18/value", O_WRONLY);
	// in theory error checking isn't too necessary in the main loop because the occasional hiccup may occur and the loop can continue just fine
	while (1) {
	
		while (gpiobuffer[0] == '0') {
			filedesc17 = open ("/sys/class/gpio/gpio17/value", O_RDONLY);
        		read (filedesc17, gpiobuffer, 16);
        		close (filedesc17);
		}
		printf("[ PRESS ]\r\n");
		while (gpiobuffer[0] == '1') {
			filedesc17 = open ("/sys/class/gpio/gpio17/value", O_RDONLY);
        		read (filedesc17, gpiobuffer, 16);
        		close (filedesc17);
		}
		printf("[ RELEASE ]\r\n"); 

		ledstate = (ledstate == ON)?OFF:ON;

		if (ledstate == ON){
			write(filedesc18,"1",1);
		}
		else {
			write(filedesc18,"0",1);
		}
	
	}

	return 0;
}

