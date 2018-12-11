import smbus
import time
import requests
import sys
import RPi.GPIO as GPIO

# img filename management
import re

from bs4 import BeautifulSoup

GPIO_LED = 17
GPIO_BUTTON = 27

MAXCOLORS = 6

def urlMaker(service):
    if (service == 'G'):
        # example url:
        # https://artsexperiments.withgoogle.com/artpalette/colors/8e8c4c-b77646-51657c
 
        url = "https://artsexperiments.withgoogle.com/artpalette/colors/"
        for col in colorList:
            url = url + col
            url = url + "-"

        # removes last char: last color should not have a dash
        url=url[:-1]

 
    elif (service == 'M'):
        # example url:
        # https://labs.tineye.com/multicolr/#colors=fee1dc,6abbd3,cb6092;weights=33,34,33;
        url="https://labs.tineye.com/multicolr/#colors="

        for col in colorList:
            url = url +(col)
            url = url +(",")

        # removes last char: last color should not have a comma
        url=url[:-1]

        url.append(";weights=")
        for col in colorList:
            url = url + (100/len(colorList))
            url = url + (",")
        
        # removes last char: last weight should not have a comma
        url=url[:-1]

        url= url +(";")

    else:
        url = "https://i.redd.it/6ipk5ua5ch611.png"

    return url

def imageParse(parenturl,maxImg):

        # from ://stackoverflow.com/questions/18408307/how-to-extract-and-download-all-images-from-a-website-using-beautifulsoup
        webpage = requests.get(parenturl)

    	soup = BeautifulSoup(webpage.text, 'html.parser')
    	img_tags = soup.find_all('img')

    	urls = [img['src'] for img in img_tags]


        # maxImg limit -- only take first maxImg urls
        if len(urls) > maxImg:
            urls = urls[:maxImg]

        # enumerate() allows access to index as well as item
    	for index, url in enumerate(urls):
                print "Attempting to parse " + url
		filename = "pics/pic" + str(index) + ".png"
		with open(filename, 'wb') as f:
                        if 'http' not in url:
                            url = '{}{}'.format(parenturl,url)
			response = requests.get(url)
        		f.write(response.content)
                        print"File " + filename + " written"

	return len(urls)

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
            
            rawcolor = "Raw data: %s\n" % (data)

            clear = clear = data[1] << 8 | data[0]
            red = data[3] << 8 | data[2]
            red8 = red / 256

            green = data[5] << 8 | data[4]
            green8 = green / 256

            blue = data[7] << 8 | data[6]
            blue8 = blue / 256
            
            crgb16 = "16bit CRGB -- C: %s, R: %s, G: %s, B: %s\n" % (clear, red, green, blue)
            rgb8 = "8bit RGB -- R: %s, G: %s, B: %s\n" % (red8, green8, blue8)
            print crgb16
            print rgb8
            print rawcolor

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

    myUrl=urlMaker(mode)
    print myUrl

    # get up to 5 images
    #imageParse(myUrl, 20)
    imageParse("https://itsalwayssunny.fandom.com/wiki/Charlie_Kelly",20)

else: 
 print "Device not found\n"


