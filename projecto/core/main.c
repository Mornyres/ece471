int main (int argc, char** argv[]){
	int fd_i2c;
	int result;
	int colorbuffer;

	char i2c_device[] = "/dev/i2c-1";
nt fd_gpio = open ("/sys/class/gpio/export", O_WRONLY);

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

f ((fd_i2c = open(i2c_device,O_RDWR)) == -1){
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
		printf("Could not enable oscillatior\r\n");
		exit(7);
	}

	buffer[0]=(0x8<<4)|(0x1);
	if ((result = write(fd_i2c, buffer, 1)) < 0){
		printf("Could not enable display\r\n");
		exit(8);
	}

	
	
	while (1){
		
		while (noColor){
		}
		getColor();
		output(); 	// debug info
		response();	// actual effect (LED)

	}
}
