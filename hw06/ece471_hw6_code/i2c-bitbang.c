#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

#include "gpio.h"
#include "i2c-bitbang.h"

// for setting SCL/SDA high or low
#define RETURN_HIGH 0
#define RETURN_LOW -1

/* Global variables */
static int scl_fd,sda_fd;

/* Pull the SDA gpio pin low */
static int SDA_gpio_pull_low(void) {

	int result=0;

	/* This function should set the SDA GPIO direction to out */
	/* and then write the value '0' to sda_fd		  */

	/* MY CODE HERE */
	gpio_set_output(SDA_GPIO); 	// set SDA gpio to output
	result = write(sda_fd,"0",1); 	// write 0 to file desc. 

	/* return the result of the write() call */

	return result;

}

/* Pull the SCL gpio pin low */
static int SCL_gpio_pull_low(void) {

	int result=0;

	/* This function should set the GPIO direction to write */
	/* and then write the value '0' to sda_fd		*/

	/* MY CODE HERE */
	gpio_set_output(SCL_GPIO);	// set SCL output
	result = write(scl_fd,"0", 1);	// write 0 to file desc.

	/* return the result of the write() call */

	return result;

}

/* Let SDA float high */
static int SDA_gpio_float_high(void) {

	int result=0;

	/* This function should set the SDA GPIO direction to input	*/
	/* Which allows the open collector bus to float high		*/

	/* MY CODE HERE */
	result = gpio_set_input(SDA_GPIO); // set SDA input

	return result;

}

/* Let SCL float high */
static int SCL_gpio_float_high(void) {

	int result=0;

	/* This function should set the SCL GPIO direction to input	*/
	/* Which allows the open collector bus to float high		*/

	/* MY CODE HERE */
	result = gpio_set_input(SCL_GPIO); // set SCL input

	return result;

}

/* read the value of SDA */
static int read_SDA(void) {

	unsigned char value=0;

	/* This function should set the GPIO direction to read	*/
	/* rewind the file descriptor, then read SDA.		*/
	/* remember to convert from ASCII before returning.	*/

	/* MY CODE HERE */
	int result = 0;
	gpio_set_input(SDA_GPIO); // set SDA input

	result = lseek(sda_fd,0,SEEK_SET); // move read file offset to 0 (start of file)
	if (result == -1){
		printf("ERR: lseek() failed\r\n");
		exit(result);
	}

	result = read(sda_fd, &value, 1); // Read a single byte from sda_fd
	if(result == -1) {
		printf("ERR: Could not read SDA input\r\n");
		exit(result);
	}

	value = value - 0x30; // convert from ASCII
	
	return value;
}

/* read the value of SCL */
static int read_SCL(void) {

	unsigned char value=0;

	/* This function should set the GPIO direction to read	*/
	/* rewind the file descriptor, then read SCL		*/
	/* remember to convert from ASCII before returning.	*/

	/* MY CODE HERE */
	int result = 0;
	gpio_set_input(SCL_GPIO);
	
	lseek(scl_fd,0,SEEK_SET);  // reset read offset to start of file

	result = read(scl_fd, &value, 1); // get SCL value from file desc.

	if(result == -1) {
		printf("ERR: Could not read SCL input\r\n");
		exit(result);
	}

	value = value - 0x30; // convert from ASCII

	return value;
}



static int I2C_delay(void) {

	usleep(4);	/* 4us, although due to sysfs overhead */
			/* and inherent usleep() limitations   */
			/* we are going to be much slower.     */

	return 0;

}


static void i2c_start_bit(void) {

	/* For a start bit, SDA goes from high to low while SCL is high */

	/* MY CODE HERE */
	int result = 0;

	result = SDA_gpio_float_high();	// SDA float high, error check
	if(result != RETURN_HIGH){
		printf("ERR: Couldn't set SDA high\r\n");
		exit(result);
	}

	result = SCL_gpio_float_high(); // SCL float high, error check
	if(result != RETURN_HIGH){
		printf("ERR: Couldn't set SCL high\r\n");
		exit(result);
	}

	result = SDA_gpio_pull_low();	// SDA pull low, error check
	if(result != RETURN_LOW){
		printf("ERR: Couldn't set SDA low\r\n");
		exit(result);
	}

	I2C_delay();
	return;
}


static void i2c_stop_bit(void) {

	/* For a stop bit, SDA goes from low to high while SCL is high */

	/* MY CODE HERE */
	int result = 0;

	result = SDA_gpio_pull_low();	// SDA pull low, error check
	if(result != RETURN_LOW) {
		printf("ERR: Couldn't set SDA low\r\n");
	
		exit(result);
	}
	
	result = SCL_gpio_float_high();	// SCL gloat high, error check
	if(result != RETURN_HIGH) {
		printf("ERR: Couldn't set SCL high\r\n");
		exit(result);
	}
	
	result = SDA_gpio_float_high(); // SDA float high, error check
	if(result != RETURN_HIGH) {
		printf("ERR: Couldn't set SDA high\r\n");
		exit(result);
	}

	I2C_delay();
	return;

}

static int i2c_read_bit(void) {

	int bit=0;

	/* Let SDA and SCL go high.  Then read SDA.  The set SCL low */

	/* MY CODE HERE */
	int result = 0;

	result = SCL_gpio_float_high();	// SCL float high, error check
	if(result != RETURN_HIGH) {
		printf("ERR: Couldn't set SCL high\r\n");
		exit(result);
	}

	I2C_delay();
	
	result = SDA_gpio_float_high();	// SDA float high, error check
	if(result != RETURN_HIGH) {
		printf("ERR: Couldn't set SDA high\r\n");
		exit(result);
	}

	I2C_delay();

	bit = read_SDA();

	return bit;

}

static int i2c_write_bit(int bit) {

	/* Pull SCL low.  Set sda to the value you want. */
	/* Then toggle SCL high then low */

	/* MY CODE HERE */
	int result = 0;

	result = SCL_gpio_pull_low(); // SCL pull low, error check
	if(result != RETURN_LOW) {
		printf("ERR: Couldn't set SCL low\r\n");
		exit(result);
	}

	switch (bit)		// SDA goes high or low according to bit value
	{
		case 0:
			result = SDA_gpio_pull_low();
			if(result != RETURN_LOW) {
				printf("ERR: Couldn't set SDA low\r\n");
				exit(result);
			}

			break;
		case 1:
			result = SDA_gpio_float_high();
			if(result != RETURN_HIGH) {
				printf("ERR: Couldn't set SDA high\r\n");
				exit(result);
			}

			break;
		default:	// bit was not 0 or 1 as expected
			printf("ERR: Invalid value %d for \"bit\" \r\n", bit);
			exit(bit);
	}
	I2C_delay();

	result = SCL_gpio_float_high();
	if(result != RETURN_HIGH){
		printf("ERR: Couldn't set SCL high\r\n");
		exit(result);
	}
	
	I2C_delay();

	result = SCL_gpio_pull_low();
	if(result != RETURN_LOW) {
		printf("ERR: Couldn't set SCL low\r\n");
		exit(result);
	}

	I2C_delay();
		
	return 0;

}

	/* Write 8 bits out and check for NACK */
static int i2c_write_byte(unsigned char byte) {

	int nack=1;

	/* MY CODE HERE */	
	for (int i=7; i>=0; i--){	// shift to i-th position starting with MSB and descending
		i2c_write_bit((byte >> i) & 0x01); // AND with 0x01 to isolate bit
	}

	nack = i2c_read_bit();
	if(nack != 0){
		printf("ERR: nack bit did not receive data correctly\r\n");
		exit(nack);
	}

	return nack;
}


	/* Write a series of bytes */
int write_i2c(unsigned char *bytes, int number) {

	int i;
	int result=0;

	i2c_start_bit();

	for(i=0;i<number;i++) {
		result=i2c_write_byte(bytes[i]);
		if (result<0) {
			printf("Error writing at location %d\n",i);
			break;
		}

	}

	i2c_stop_bit();

	return result;

}


/* Initialize two file descriptors to be used for SDA and SCL */
/* They are global variables to make the code a bit easier    */
int i2c_init_gpios(int sda, int scl) {

        char string[BUFSIZ];

	/* Enable the GPIOs */
	if (gpio_enable(sda)<0) printf("Error enabling SDA\n");
	if (gpio_enable(scl)<0) printf("Error enabling SCL\n");

	/* Open the SDA fd */
	printf("Using GPIO%d as SDA\n",sda);
        sprintf(string,"/sys/class/gpio/gpio%d/value",sda);
        sda_fd=open(string,O_RDWR);
        if (sda_fd<0) {
                fprintf(stderr,"\tError getting value!\n");
                return -1;
        }

	/* Open the SCL fd */
	printf("Using GPIO%d as SCL\n",scl);
	sprintf(string,"/sys/class/gpio/gpio%d/value",scl);

        scl_fd=open(string,O_RDWR);
        if (scl_fd<0) {
                fprintf(stderr,"\tError getting value!\n");
                return -1;
        }

	/* Set the read/write direction of the GPIOs */
	gpio_set_input(sda);
	gpio_set_input(scl);

	return 0;
}
