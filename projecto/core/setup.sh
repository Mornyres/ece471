sudo modprobe i2c_dev
sudo i2cdetect -y -r 1

sudo pigpiod
sudo bash -c "export PIGPIO_ADDR=soft; echo \$PIGPIO_ADDR"
sudo bash -c "export PIGPIO_PORT=8888; echo \$PIGPIO_PORT"

sudo echo "auto lo" >> /etc/network/interfaces
sudo echo "iface lo inet loopback" >> /etc/network/interfaces



echo "No errors?"
