import smbus
import time

import RPi.GPIO as GPIO
#https://learn.sparkfun.com/tutorials/raspberry-gpio/python-rpigpio-api

GPIO_LED = 17
GPIO_BUTTON = 27

# number by GPIO, not by physical pin number
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

GPIO.setup(GPIO_LED, GPIO.OUT)
GPIO.setup(GPIO_BUTTON, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

bus = smbus.SMBus(1)
# I2C address 0x29
# Register 0x12 has device ver. 
# Register addresses must be OR'ed with 0x80
bus.write_byte(0x29,0x80|0x12)
ver = bus.read_byte(0x29)
# version # should be 0x44
if ver == 0x44:
    print "Device found\n"
    bus.write_byte(0x29, 0x80|0x00) # 0x00 = ENABLE register
    bus.write_byte(0x29, 0x01|0x02) # 0x01 = Power on, 0x02 RGB sensors enabled
    bus.write_byte(0x29, 0x80|0x14) # Reading results start register 14, LSB then MSB

    # turn off led
    GPIO.output(GPIO_LED, GPIO.LOW)

    while True:

        #proceed iff button pressed
        GPIO.wait_for_edge(GPIO_BUTTON, GPIO.FALLING)  
        # turn on led
        GPIO.output(GPIO_LED, GPIO.HIGH)

        data = bus.read_i2c_block_data(0x29, 0)
        clear = clear = data[1] << 8 | data[0]
        red = data[3] << 8 | data[2]
        green = data[5] << 8 | data[4]
        blue = data[7] << 8 | data[6]
        crgb = "C: %s, R: %s, G: %s, B: %s\n" % (clear, red, green, blue)
        print crgb

        #proceed iff button NOT pressed
        GPIO.wait_for_edge(GPIO_BUTTON, GPIO.RISING)  

        # turn off led
        GPIO.output(GPIO_LED, GPIO.LOW)

else: 
 print "Device not found\n"
