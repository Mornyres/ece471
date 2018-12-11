/* Default i2c GPIO output pins on Raspberry-Pi */
#define SDA_GPIO	2
#define SCL_GPIO	3

int write_i2c(unsigned char *bytes, int number);
int i2c_init_gpios(int sda, int scl);
