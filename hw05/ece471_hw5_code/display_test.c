#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/ioctl.h>

#include <linux/i2c-dev.h>

int main(int argc, char **argv) {

	int fd;
	int status, result;

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
	/* Write 1s to all Display Columns */
// YOUR CODE HERE

	int i;
	buffer[0]=0x0;
	for(i=0;i<16;i++) buffer[1+i]=0xff;
	if ((result = write(fd, buffer, 17)) < 0){
		printf("Could not write to display\r\n");
		exit(5);
	}

	/* Close device */
// YOUR CODE HERE

	close(fd);

	return 0;
}
