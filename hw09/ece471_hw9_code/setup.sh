sudo modprobe i2c_bcm2708
sudo modprobe i2c_dev
sudo i2cdetect -y -r 1

sudo modprobe w1-gpio
sudo modprobe w1-therm

echo "No errors?"
