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
#define DECIMAL 0x80

// only G enabled
#define char_negsymb 0x40
// 0b00000001

#define DEBOUNCE_S 0.5	// very crappy extra debounce delay

// for aligning output, may want to change
#define PLACE1	1
#define PLACE2	3
#define PLACE3	7
#define PLACE4	9

int tempDisplay(double tempInput) {

	int num_array[10] = {char_0, char_1, char_2, char_3, char_4, char_5, char_6, char_7, char_8, char_9};

	char display_buffer[3]={0,0,0};

	int fd_i2c;
	int result;
	
	// set tens AND ones places
	int tempHundreds = ((int)tempInput / 100);
	tempHundreds=abs(tempHundreds);

	int tempTens = ((int)tempInput / 10) % 10;
	tempTens = abs(tempTens);

	int tempOnes = (((int)tempInput % 100) % 10);
	tempOnes=abs(tempOnes);

	double remainder = tempInput - (int)tempInput;

	// add 0.1 just because casting to int will take the floor (2 may be encoded as 1.9999... where casting makes it 1)
	int tempTenths = (int)(( remainder * 10.0) + 0.1);
	tempTenths=abs(tempTenths);


	// if -99.9 to 99.9
	if (tempHundreds == 0){
		// if pos
		if (tempInput >= 0){
			//firstplace=10s
			
			// suppress leading 0
			if (tempTens==0){
				display_buffer[0]=0x0;
			}
			else{
				display_buffer[0]=num_array[tempTens];
			}

			//second = 1s
			// ORing with decimal bit(0x80) to attach decimal to ones place
			display_buffer[1]=num_array[tempOnes] | DECIMAL;
			// (decimal)
			display_buffer[2]=num_array[tempTenths];
		}
		else{ 	// lower than 0
			// first = neg symb
			display_buffer[0]=char_negsymb;
			// second = 10s
			display_buffer[1]=num_array[tempTens];
			// third = 1s
			display_buffer[2]=num_array[tempOnes];
		}
	}

	// if 100 to 999
	else if ((tempHundreds<=9) && (tempHundreds >0)){
		// first = 100s
		display_buffer[0]=num_array[tempHundreds];
		// second = 10s
		display_buffer[1]=num_array[tempTens];
		// third = 1s
		display_buffer[2]=num_array[tempOnes];
	}

	else{
		printf("Invalid temp. input\r\n");
		exit(1);
	}
		
	printf("DEBUG: \r\n"
			"Hundreds --  %d \r\n"
			"Tens -- %d \r\n"
			"Ones -- %d \r\n"
			"Tenths -- %d \r\n",tempHundreds,tempTens,tempOnes,tempTenths);
	printf("DEBUG: Displaying %X | %X | %X\r\n",display_buffer[0],display_buffer[1],display_buffer[2]);

	char i2c_device[]="/dev/i2c-1";

	unsigned char buffer[17];

	// enable GPIO17
	int fd_gpio = open ("/sys/class/gpio/export", O_WRONLY);

	if (fd_gpio < 0){
		printf("Could not open GPIO export file\r\n");
		exit(2);
	}
	write (fd_gpio, "17", 2); 
	close (fd_gpio);

	// set GPIO17 to input
	fd_gpio = open("/sys/class/gpio/gpio17/direction", O_WRONLY);
	if (fd_gpio < 0){
		printf("Could not open GPIO direction file\r\n");
		exit(3);
	}
	write (fd_gpio, "in", 2); 
	close (fd_gpio);

	// read initial value of GPIO17
	char gpiobuffer[16]; 
	fd_gpio = open ("/sys/class/gpio/gpio17/value", O_RDONLY);
	if (fd_gpio < 0){
		printf("Could not open GPIO value file\r\n");
		exit(4);
	}
        read (fd_gpio, gpiobuffer, 16);
        close (fd_gpio);
	
	/* Open i2c device */
	if ((fd_i2c = open(i2c_device,O_RDWR)) == -1){
		printf("Could not open device\r\n");
		exit(5);
	}
	/* Set slave address */
	if ((result = ioctl(fd_i2c,I2C_SLAVE, 0x70)) < 0){
		printf("Could not set slave address\r\n");
		exit(6);
	}
	
	/* Turn on oscillator */
	buffer[0]=(0x02<<4) | (0x01);
	if ((result = write(fd_i2c, buffer, 1)) < 0){
		printf("Could not enable oscillator\r\n");
		exit(7);
	}
	/* Turn on Display, No Blink */

	buffer[0]=(0x8<<4)|(0x1);
	if ((result = write(fd_i2c, buffer, 1)) < 0){
		printf("Could not enable display\r\n");
		exit(8);
	}

	/* Set Brightness */

	buffer[0]=(0xe<<4)|(0x9); 		// Set brightness to 10/16 DC
	if ((result = write(fd_i2c, buffer, 1)) < 0){
		printf("Could not set brightness\r\n");
		exit(9);
	}
	/* Write 1s to all Display Columns */

	int i;
	buffer[0]=0x0;
	for(i=0;i<16;i++) buffer[1+i]=0xff;
	if ((result = write(fd_i2c, buffer, 17)) < 0){
		printf("Could not write to display\r\n");
		exit(10);
	}
	
	char numdisplay[16] = {0, 0, 0, 0, 0, 0, 0, char_0, 0, 0, 0, 0, 0, 0, 0, 0};
		
	/* Clear Screen */
	for(i=0;i<16;i++) buffer[1+i]=0x00;
	result = write(fd_i2c, buffer, 17);
	if(result < 0) {
		printf("Error clearing screen\n"); 
		exit(11);
	}
	usleep(DEBOUNCE_S*1000000); 	// debounce

	// hundreds place	
	numdisplay[PLACE1] = display_buffer[0]; // index to 7seg code array defined above and place in rightmost spot	
	
	// tens place
	numdisplay[PLACE2] = display_buffer[1]; // index to 7seg code array defined above and place in rightmost spot	

	// ones place
	numdisplay[PLACE3] = display_buffer[2]; // index to 7seg code array defined above and place in rightmost spot	

	// degree symbol
	numdisplay[PLACE4] = char_deg; 

	for(i=0;i<16;i++) buffer[i]=numdisplay[i];
	result = write(fd_i2c, buffer, 17);
	if(result < 0) {
		printf("Error writing temp\r\n"); 
		exit(12);
	}
	
	usleep(DEBOUNCE_S*1000000); 	// debounce
	

	close(fd_i2c);

	return 0;
}

#endif
/*define LCD_H_*/
