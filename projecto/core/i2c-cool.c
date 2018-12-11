#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

#include "i2c-bitbang.h"

#define char_0 0x3f
#define char_1 0x06
#define char_2 0x5b
#define char_3 0x4f
#define char_4 0x66
#define char_5 0x6d
#define char_6 0x7d
#define char_7 0x07
#define char_8 0x7f
#define char_9 0x6f

int main(int argc, char **argv) {

	int num_array[10] = {char_0, char_1, char_2, char_3, char_4, char_5, char_6, char_7, char_8, char_9};
	int result;
	unsigned char buffer[17];
	int i = 0;
	int count = 0;
/* BEGIN SOMETHING COOL SETUP */
	// enable GPIO17
	int fd_gpio = open ("/sys/class/gpio/export", O_WRONLY);

	if (fd_gpio < 0){
		printf("Could not open GPIO export file\r\n");
		exit(1);
	}
	write (fd_gpio, "17", 2); 
	close (fd_gpio);

	// set GPIO17 to input
	fd_gpio = open("/sys/class/gpio/gpio17/direction", O_WRONLY);
	if (fd_gpio < 0){
		printf("Could not open GPIO direction file\r\n");
		exit(2);
	}
	write (fd_gpio, "in", 2); 
	close (fd_gpio);

	// read initial value of GPIO17
	char gpiobuffer[16]; 
	fd_gpio = open ("/sys/class/gpio/gpio17/value", O_RDONLY);
	if (fd_gpio < 0){
		printf("Could not open GPIO value file\r\n");
		exit(3);
	}
        read (fd_gpio, gpiobuffer, 16);
        close (fd_gpio);
/* END SOMETHING COOL SETUP */	

	/*******************************************/
	/* Init the GPIO lines we are going to use */
	/* to bitbang the i2c                      */
	/*******************************************/
	result=i2c_init_gpios(SDA_GPIO,SCL_GPIO);
	if (result<0) {
		exit(1);
	}


	/****************************************/
	/****************************************/
	/* Start the display			*/
	/****************************************/
	/****************************************/

	/* Address of device, and we want to write */
        buffer[0]= 0x70<<1 | 0x0;
	/* Command 0x20 = SYSTEM SETUP  (See p30 of manual) */
	/* Action  0x01 = Turn on oscillator */
        buffer[1]= 0x20 | 0x01;

	/* Write the two bytes in our buffer */
	result=write_i2c(buffer,2);
        if ( result<0 ) {
                fprintf(stderr,"Error starting display (enable oscilator)!\n");
                return -1;
        }

	/****************************************/
	/****************************************/
	/* Turn display on, no blinking		*/
	/****************************************/
	/****************************************/

	/* Address of device, and we want to write */
        buffer[0]= 0x70<<1 | 0x0;

	/* Command = 0x80 Setup*/
	/* Action = Blinking off, display on */
        buffer[1]= 0x80 | 0x1;

	/* Write the two bytes in our buffer */
	result=write_i2c(buffer,2);
        if ( result<0 ) {
                fprintf(stderr,"Error starting display (turning on)!\n");
                return -1;
        }



	/****************************************/
	/****************************************/
	/* Set Brightness			*/
	/****************************************/
	/****************************************/

	/* Address of device, and we want to write */
        buffer[0]= 0x70<<1 | 0x0;

	/* Command = 0xe0 Dimming */
	/* Action = Brightness = 10/16 */
        buffer[1]= 0xe0 | 0xa;

	/* Write the two bytes in our buffer */
	result=write_i2c(buffer,2);
        if ( result<0 ) {
                fprintf(stderr,"Error setting brightness!\n");
                return -1;
        }
	

	buffer[0] = 0x0;
	for (i=0; i<16;i++) buffer[1+i]=0xff;
	result=write_i2c(buffer,17);
        if ( result<0 ) {
                fprintf(stderr,"Could not clear display\n");
                return -1;
        }

	char numdisplay[16] = {0, 0, 0, 0, 0, 0, 0, char_0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0};

while (1) {
	
		while (gpiobuffer[0] == '0') {
			fd_gpio = open ("/sys/class/gpio/gpio17/value", O_RDONLY);
        		read (fd_gpio, gpiobuffer, 16);
        		close (fd_gpio);
		}
		printf("[ PRESS ]\r\n");
		while (gpiobuffer[0] == '1') {
			fd_gpio = open ("/sys/class/gpio/gpio17/value", O_RDONLY);
        		read (fd_gpio, gpiobuffer, 16);
        		close (fd_gpio);
		}
		printf("[ RELEASE ]\r\n"); 
		
		usleep(1*1000000); 	// I have a hardware-debounced button, and the below code probably takes care of it, but I'm throwing in this delay anyway after each press to help avoid weird behavior
	

		/* Clear Screen */
		for(i=0;i<16;i++) buffer[1+i]=0x00;
		result = write_i2c(buffer, 17);
		if(result < 0) {
			printf("Error clearing screen\n"); 
			exit(5);
		}
		usleep(1*1000000); 	//Wait for 1 second
		
		/* SOMETHING COOL: select a number to write */
		if (count++ > 9) count = 0; // instead of doing extra work to make this work for 10 and so on, just reset after 9
		
		numdisplay[7] = num_array[count]; // index to 7seg code array defined above and place in rightmost spot	

		/* Write current count */ 
		for(i=0;i<16;i++) buffer[i]=numdisplay[i];
        	result = write_i2c(buffer, 17);
		if(result < 0) {
                	printf("Error writing count\n"); 
			exit(6);
        	}
		
		usleep(1*1000000); 	//Wait for 1 second
	
	}

	return 0;
}
