import smbus
import time

# also needs ordered_dict in some cases
from collections import OrderedDict
#import requests

#argv and encoding
import sys

# following https://pythonadventures.wordpress.com/tag/unicodeencodeerror/ for below.
# unicode encoding problems.
reload(sys)
sys.setdefaultencoding("utf-8")

# comes with Raspbian
import RPi.GPIO as GPIO

from myfunctions import javaParse
from myfunctions import urlMaker

GPIO_LED = 17
GPIO_BUTTON = 27

MAXCOLORS = 6
NUM_PICS = 6

# theoretical max value for raw data -- [ticks of int. clock] * [int clock period]
# currently not used but may be useful at some point
MAX16=21*1024


# enter main loop
mode = 'G'
if (len(sys.argv) == 2):
    mode = sys.argv[1]

# empty list for colors to be added to
colorList = []

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

    i = 0

    while i < MAXCOLORS:

        try:
            #proceed iff button pressed
            #GPIO.wait_for_edge(GPIO_BUTTON, GPIO.FALLING)  
            raw_input("Press a button to start reading\n")

            # turn on led
            GPIO.output(GPIO_LED, GPIO.HIGH)
            #delay a bit
            time.sleep(1)

            data = bus.read_i2c_block_data(0x29, 0)
            
            p_rawcolor = "Raw data: %s\n" % (data)

            clear = clear = data[1] << 8 | data[0]

            # for each color: 8bit RGB value approx. equal to color divided by clear channel value for ratio, multiplied by 255 for 8bit
            # however some clear light is not accounted for by rgb channels so let's try just the sum of these 3
            red = data[3] << 8 | data[2]
            
            green = data[5] << 8 | data[4]

            blue = data[7] << 8 | data[6]
            
            blue8 = (float(blue)/float(red+blue+green)) * 255
            blue8 = int(blue8)
            
            green8 = (float(green)/float(red+blue+green)) * 255
            green8 = int(green8)
            
            red8 = (float(red)/float(red+blue+green)) * 255
            red8 = int(red8)
            
            p_crgb16 = "16bit CRGB -- C: %s, R: %s, G: %s, B: %s\n" % (clear, red, green, blue)
            p_rgb8 = "8bit RGB -- R: %s/255, G: %s/255, B: %s/255\n" % (red8, green8, blue8)
            print p_crgb16
            print p_rgb8
            print p_rawcolor


            # cast rgb to 0xRRGGBB
            currentColor = (red8 << (4*4))
            currentColor = currentColor | (green8 << (2*4))
            currentColor = currentColor | (blue8 << (0*4))
            currentColor = "{:06x}".format(currentColor)
    

            standardRGB = "RGB: %s\n" % (currentColor)
            print standardRGB
            #proceed iff button NOT pressed
            #GPIO.wait_for_edge(GPIO_BUTTON, GPIO.RISING)  
            raw_input("Press a button to finish reading\n")

            # turn off led
            GPIO.output(GPIO_LED, GPIO.LOW)

            colorList.append(currentColor)
            i = i + 1
        
        except KeyboardInterrupt:
            # turn off led
            GPIO.output(GPIO_LED, GPIO.LOW)
            break

    print "saved color list", colorList
    myUrl=urlMaker(colorList,mode)
    print "\nConstructed URL: " + myUrl

    # get up to 6 images

    #below for debugging
    myUrl = "https://artsexperiments.withgoogle.com/artpalette/colors/351e12-17913e-911b1b"
    if (javaParse(myUrl, NUM_PICS) != NUM_PICS):
        print "Did not get requested picture count\n"

else: 
 print "Device not found\n"


