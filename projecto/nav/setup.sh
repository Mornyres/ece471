echo "Note to future self (or Weaver if you're crazy):"
echo "Comment this part out after reviewing and modifying these commands. Notably, your $PATH variable may not be usr/bin."
exit 0

sudo modprobe i2c_dev
sudo i2cdetect -y -r 1

sudo pigpiod
sudo bash -c "export PIGPIO_ADDR=soft; echo \$PIGPIO_ADDR"
sudo bash -c "export PIGPIO_PORT=8888; echo \$PIGPIO_PORT"

sudo echo "auto lo" >> /etc/network/interfaces
sudo echo "iface lo inet loopback" >> /etc/network/interfaces

sudo apt-get install firefox-esr

pip install selenium

pip install smbus

pip install bs4

        # https://stackoverflow.com/questions/47782650/selenium-common-exceptions-sessionnotcreatedexception-message-unable-to-find-a?rq=1
dpkg -i chromedriver_armhf.deb
echo "No errors? I hope?"
