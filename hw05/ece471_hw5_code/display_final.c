#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/ioctl.h>

#include <linux/i2c-dev.h>

#define char_E 0x79
#define char_C 0x39
#define char_4 0x66
#define char_7 0x07
#define char_1 0x06

int main(int argc, char **argv) {

	int fd;
	int result;

	char i2c_device[]="/dev/i2c-1";

	unsigned char buffer[17];

	/* Open i2c device */
// YOUR CODE HERE
	if ((fd = open(i2c_device,O_RDWR)) == -1){
		printf("Could not open device\r\n");
		exit(1);
	}
	/* Set slave address */
// YOUR CODE HERE
	if ((result = ioctl(fd,I2C_SLAVE, 0x70)) < 0){
		printf("Could not set slave address\r\n");
		exit(2);
	}
	
	/* Turn on oscillator */
// YOUR CODE HERE
	buffer[0]=(0x02<<4) | (0x01);
	if ((result = write(fd, buffer, 1)) < 0){
		printf("Could not enable oscillator\r\n");
		exit(3);
	}
	/* Turn on Display, No Blink */
// YOUR CODE HERE

	buffer[0]=(0x8<<4)|(0x1);
	if ((result = write(fd, buffer, 1)) < 0){
		printf("Could not enable display\r\n");
		exit(4);
	}

	/* Set Brightness */
// YOUR CODE HERE

	buffer[0]=(0xe<<4)|(0x9); 		// Set brightness to 10/16 DC
	if ((result = write(fd, buffer, 1)) < 0){
		printf("Could not set brightness\r\n");
		exit(4);
	}
	/* Write "ECE 471" to screen */
// YOUR CODE HERE
	int i; 
	char array_ECE[16] = {0, char_E, 0, char_C, 0, 0, 0, char_E, 0, 0, 0, 0, 0, 0, 0, 0}; // ‘ECE’
	char array_471[16] = {0, char_4, 0, char_7, 0, 0, 0, char_1, 0, 0, 0, 0, 0, 0, 0, 0}; // ‘471’

	/* Clear Screen */
	for(i=0;i<16;i++) buffer[1+i]=0x00;
        result = write(fd, buffer, 17);
	if(result < 0) {
               	printf("Error clearing screen\n"); 
		exit(5);
       	}

	while (1) {
		/* Write ECE */ 
		for(i=0;i<16;i++) buffer[i]=array_ECE[i];
        	result = write(fd, buffer, 17);
		if(result < 0) {
                	printf("Error writing ECE\n"); 
			exit(6);
        	}
		
		usleep(1*1000000); 	//Wait for 1 second

		/* Write 471 */
		for(i=0;i<16;i++) buffer[i]=array_471[i];
        	result = write(fd, buffer, 17);
		if(result < 0) {
                	printf("Error writing 471\n"); 
			exit(7);
        	}

		usleep(1*1000000); 	//Wait for 1 second
	}
	/* Close device */
// YOUR CODE HERE

	close(fd);

	return 0;
}
