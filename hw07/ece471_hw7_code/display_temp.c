#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define LENGTH 3

int main(int argc, char **argv) {

        int spi_fd, spi_mode, spi_result, i;
	struct spi_ioc_transfer spi1;
	unsigned char buffer_out[LENGTH]={0x1,0x2,0x3};
	unsigned char buffer_in[LENGTH];

	double output_C1, output_C0; 

	/* kernel doesn’t like it if stray values, even in padding */
        memset (&spi1, 0, sizeof(struct spi_ioc_transfer));

        /* Setup full-duplex transfer of 3 bytes */
        spi1.tx_buf = (unsigned long)&buffer_out;
        spi1.rx_buf = (unsigned long)&buffer_in;
        spi1.len = LENGTH;
        spi1.delay_usecs = 0;
        spi1.speed_hz = 100000;
        spi1.bits_per_word = 8;
        spi1.cs_change = 0;

	/* Open SPI device */
	spi_fd = open("/dev/spidev0.0",O_RDWR);
	if (spi_fd == 0){
		printf("ERR: Couldn't open device\r\n");
		exit(1);
	}

	/* Set SPI Mode_0 */
	spi_mode = SPI_MODE_0;

	spi_result = ioctl(spi_fd, SPI_IOC_WR_MODE, &spi_mode);
	if (spi_result < 0){
		printf("ERR: Couldn' set SPI mode\r\n");
		exit(2);
	}

	/* Loop forever printing the CH0 and CH1 Voltages 	*/
	/* Once per second.					*/
	while(1){

		/* Be sure to clear out your transmit buffer with zeroes first (using memset or similar)
or you may get errors! */
		for (i=0; i<3; i++)	buffer_out[i] = 0x0;

		/* Byte 0: start bit | B1: 0b1 [3bit channel] | B2: zeroes */
		buffer_out[0] = 0x01;
		buffer_out[1] = 0x80;	// select channel 0 (0b1000)
		buffer_out[2] = 0x00;

		/* Receive 3 bytes. The first byte can be ignored, the bottom 2 bits of the second byte
are bits 9 and 8, and the third byte is the bottom 8 bits of the result */	
		/* Use the SPI_IOC_MESSAGE(1) ioctl() as described in the class notes */
		spi_result = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi1);
		
		/* ignore byte 0, take 2 LSB of byte 1 and shift to bit 8 in result, add to    */
		output_C0 = 	( ( (buffer_in[1]) << 8) 
				+ buffer_in[2])
				& 0x03ff; // AND with 0b1111111111 for only bits 0-9
		/* Be sure to clear out your transmit buffer with zeroes first (using memset or similar)
or you may get errors! */
		for (i=0; i<3; i++)	buffer_out[i] = 0x0;

		buffer_out[0] = 0x01;
		buffer_out[1] = 0x90;	// select channel 1
		buffer_out[2] = 0x00;

		/* Receive 3 bytes. The first byte can be ignored, the bottom 2 bits of the second byte
are bits 9 and 8, and the third byte is the bottom 8 bits of the result */	
		/* Use the SPI_IOC_MESSAGE(1) ioctl() as described in the class notes */
		spi_result = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi1);
			
		output_C1 = 	( ( (buffer_in[1]) << 8) 
				+ buffer_in[2])
				& 0x03ff; // AND with 0b1111111111 for only bits 0-9
		
		/* Now that channels 0 and 1 each have been read, convert */
		output_C0 = (output_C0 * 3.3) / 1024.0;
		output_C1 = (output_C1 * 3.3) / 1024.0;

		/* convert to temp. */
		output_C0 = (100.0 * output_C0) - 50.0;
		output_C1 = (100.0 * output_C1) - 50.0;

		/* fahrenheit is stupid so I'm not bothering >B^) */
		/* print values with 2 digits after decimal */
		printf("CHANNEL 0: %.2lf deg C \r\nCHANNEL 1: %.2lf deg C \r\n",output_C0,output_C1);
		
		usleep(1 * 1000000);
		
	}
	

	return 0;
}
