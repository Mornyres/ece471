#ifndef LCD_H_
#define LCD_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <string.h>

// 7seg codes for each digit to be displayed:
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

// segs A,B,F,G enabled
#define char_deg 0x63
// 0b01100011

#define DEBOUNCE_S 0.5	// very crappy extra debounce delay

// for aligning output, may want to change
#define HUNDREDSPLACE	1
#define TENSPLACE	3
#define ONESPLACE	7
#define DEGPLACE	9

int tempDisplay(int tempInput) {

	int num_array[10] = {char_0, char_1, char_2, char_3, char_4, char_5, char_6, char_7, char_8, char_9};

	int fd_i2c;
	int result;
	
	// set tens AND ones places
	int tempHundreds = (tempInput / 100);
	int tempTens = (tempInput / 10) % 10;
	int tempOnes = ((tempInput % 100) % 10) ;

	printf("DEBUG: \r\n"
			"Hundreds --  %d \r\n"
			"Tens -- %d \r\n"
			"Ones -- %d \r\n",tempHundreds,tempTens,tempOnes);

	char i2c_device[]="/dev/i2c-1";

	unsigned char buffer[17];

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
	
	/* Open i2c device */
// YOUR CODE HERE
	if ((fd_i2c = open(i2c_device,O_RDWR)) == -1){
		printf("Could not open device\r\n");
		exit(4);
	}
	/* Set slave address */
// YOUR CODE HERE
	if ((result = ioctl(fd_i2c,I2C_SLAVE, 0x70)) < 0){
		printf("Could not set slave address\r\n");
		exit(5);
	}
	
	/* Turn on oscillator */
// YOUR CODE HERE
	buffer[0]=(0x02<<4) | (0x01);
	if ((result = write(fd_i2c, buffer, 1)) < 0){
		printf("Could not enable oscillator\r\n");
		exit(6);
	}
	/* Turn on Display, No Blink */
// YOUR CODE HERE

	buffer[0]=(0x8<<4)|(0x1);
	if ((result = write(fd_i2c, buffer, 1)) < 0){
		printf("Could not enable display\r\n");
		exit(7);
	}

	/* Set Brightness */
// YOUR CODE HERE

	buffer[0]=(0xe<<4)|(0x9); 		// Set brightness to 10/16 DC
	if ((result = write(fd_i2c, buffer, 1)) < 0){
		printf("Could not set brightness\r\n");
		exit(8);
	}
	/* Write 1s to all Display Columns */
// YOUR CODE HERE

	int i;
	buffer[0]=0x0;
	for(i=0;i<16;i++) buffer[1+i]=0xff;
	if ((result = write(fd_i2c, buffer, 17)) < 0){
		printf("Could not write to display\r\n");
		exit(9);
	}
	
	char numdisplay[16] = {0, 0, 0, 0, 0, 0, 0, char_0, 0, 0, 0, 0, 0, 0, 0, 0};
		
// YOUR CODE HERE

	
	/* Clear Screen */
	for(i=0;i<16;i++) buffer[1+i]=0x00;
	result = write(fd_i2c, buffer, 17);
	if(result < 0) {
		printf("Error clearing screen\n"); 
		exit(5);
	}
	usleep(DEBOUNCE_S*1000000); 	// debounce

	// hundreds place	
	numdisplay[HUNDREDSPLACE] = num_array[tempHundreds]; // index to 7seg code array defined above and place in rightmost spot	
	
	// tens place
	numdisplay[TENSPLACE] = num_array[tempTens]; // index to 7seg code array defined above and place in rightmost spot	

	// ones place
	numdisplay[ONESPLACE] = num_array[tempOnes]; // index to 7seg code array defined above and place in rightmost spot	

	// degree symbol
	numdisplay[DEGPLACE] = char_deg; 

	/* Write current count */ 
	for(i=0;i<16;i++) buffer[i]=numdisplay[i];
	result = write(fd_i2c, buffer, 17);
	if(result < 0) {
		printf("Error writing count\n"); 
		exit(6);
	}
	
	usleep(DEBOUNCE_S*1000000); 	// debounce
	

	close(fd_i2c);

	return 0;
}

#endif
/*define LCD_H_*/
