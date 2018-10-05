#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>	/* open() */
#include <unistd.h>	/* close() */
#include <string.h>


int main(int argc, char **argv) {
	
	char gpiobuffer[10]; 
	
	int fdesc = open("/sys/class/gpio/export", O_WRONLY);
	
	if (fdesc < 0){  // returns -1 in event of failure 
		printf("Could not open GPIO export file");
		exit(1);
	}

	// below: enable usage of GPIO18
	strncpy(gpiobuffer,"18",2); // buffer to be written to gpio corresponds to GPIO18 
	
	write(fdesc,gpiobuffer,2); // carry out write 
	close(fdesc); 

	// set GPIO18 to output
	fdesc=open("/sys/class/gpio/gpio18/direction", O_WRONLY); 
	if (fdesc < 0){  // returns -1 in event of failure 
		printf("Could not open GPIO direction file");
		exit(2);
	}

	write(fdesc,"out",3); // carry out write
	close(fdesc); 

	// Alternate the value of GPIO 18 between 1 and 0, blinking LED
	fdesc=open("/sys/class/gpio/gpio18/value", O_WRONLY);
	
	while (1){
		strncpy(gpiobuffer, "1", 1); 	
		write(fdesc,gpiobuffer,1);	// LED on
		usleep(0.5 * 1000000); 	// Wait 0.5 seconds
		strncpy(gpiobuffer, "0", 1);	 
		write(fdesc,gpiobuffer,1);	// LED off
		usleep(0.5 * 1000000); 	// Wait 0.5 seconds
	}

	return 0;

}


