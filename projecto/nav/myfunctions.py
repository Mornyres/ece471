from selenium.webdriver.chrome.options import Options
# we need javascript support
from selenium import webdriver
import selenium.webdriver.chrome.service as service

# for parsing web pages
from bs4 import BeautifulSoup

from pyvirtualdisplay import Display

import requests

def urlMaker(colorList,site):
    if (site == 'G'):
        # example url:
        # https://artsexperiments.withgoogle.com/artpalette/colors/8e8c4c-b77646-51657c
 
        url = "https://artsexperiments.withgoogle.com/artpalette/colors/"
        for col in colorList:
            url = url + (col)
            url = url + ("-")

        # removes last char: last color should not have a dash
        url=url[:-1]

 
    elif (site == 'M'):
        # example url:
        # https://labs.tineye.com/multicolr/#colors=fee1dc,6abbd3,cb6092;weights=33,34,33;
        url="https://labs.tineye.com/multicolr/#colors="

        for col in colorList:
            url = url + (col)
            url = url +(",")

        # removes last char: last color should not have a comma
        url=url[:-1]

        # multicolr allows adjustment of weights for each color but we won't bother -- instead evenly divide 100 by the num. of colors
        url=url+";weights="
        for col in colorList:
            url = url + str(100/len(colorList))
            url = url + (",")
        
        # removes last char: last weight should not have a comma
        url=url[:-1]

        url= url +(";")

    else:
        # unrecognized method, go to the land of cursed images
        url = "https://www.reddit.com/r/cursedimages/"
    return url

def javaParse(parenturl,maxImg):

        display = Display(visible=0,size=(800,600))
        display.start()

        print "Parsing: " + parenturl
        

        driver = driverInit("chrome")

        if (driver < 0):
            return driver
        #driver = webdriver.Chrome(executable_path="/usr/lib/chromium-browser/chromedriver",chrome_options=options)
        
        driver = webdriver.PhantomJS()
        
        driver.get(parenturl)
        time.sleep(1)
        # we can now directly manipulate the page
        # https://stackoverflow.com/questions/20986631/how-can-i-scroll-a-web-page-using-selenium-webdriver-in-python
        driver.execute_script("window.scrollTo(0),document.body.scrollHeight);")

        # https://selenium-python.readthedocs.io/locating-elements.html
        webpage=driver.find_element_by_class_name('results')
        driver.quit()

        # done interacting with page

        debugFile = open("rawpage.html", 'wb')
        debugFile.write.webpage

    	soup = BeautifulSoup(webpage, 'html.parser')


        # below works for most cases but not all
    	img_tags = soup.find_all('img')

        urls = [img['src'] for img in img_tags]
        
        # format for printing to a debug file (which we wound up using a lot)
        debugFile = open("soup.html", 'wb')
        debugFile.write(soup.prettify("utf-8"))
 

        # maxImg limit -- only take first maxImg urls
        if len(urls) > maxImg:
            urls = urls[:maxImg]

        # enumerate() allows access to index as well as item
    	for index, url in enumerate(urls):
                print "Attempting to parse " + url
		filename = "pics/pic" + str(index) + ".bmp"
		with open(filename, 'wb') as f:
                        if 'http' not in url:
                            url = '{}{}'.format(parenturl,url)
			response = requests.get(url)
        		f.write(response.content)
                        print"File " + filename + " written"

	return len(urls)

# "legacy"
def imageParse(parenturl,maxImg):

        print "Parsing: " + parenturl
        # from ://stackoverflow.com/questions/18408307/how-to-extract-and-download-all-images-from-a-website-using-beautifulsoup
        webpage = requests.get(parenturl)

        # extract server response as raw data

        webpage = webpage.content

        debugFile = open("rawpage.html", 'wb')
        debugFile.write(webpage)

    	soup = BeautifulSoup(webpage, 'html.parser')


        # below works for most cases but not all
    	img_tags = soup.find_all('img')
        background_imgs = soup.find_all("background-image")

        urls = [img['src'] for img in img_tags]
        
        # handle palette cases: seems to be no decrease in quality requesting width 1920, so always do that
        print background_imgs
        # format for printing to a debug file (which we wound up using a lot)
        debugFile = open("soup.html", 'wb')
        debugFile.write(soup.prettify("utf-8"))

        # maxImg limit -- only take first maxImg urls
        if len(urls) > maxImg:
            urls = urls[:maxImg]

        # enumerate() allows access to index as well as item
    	for index, url in enumerate(urls):
                print "Attempting to parse " + url
		filename = "pics/pic" + str(index) + ".bmp"
		with open(filename, 'wb') as f:
                        if 'http' not in url:
                            url = '{}{}'.format(parenturl,url)
			response = requests.get(url)
        		f.write(response.content)
                        print"File " + filename + " written"

	return len(urls)

def driverInit(browser):

    # default return if something goes wrong
    driver = -1

    if browser == 'chrome':
        #https://stackoverflow.com/questions/47596402/selenium-chrome-failed-to-start-exited-abnormally-error
        options = webdriver.ChromeOptions()
        options.add_argument('--disable-extensions')
        options.add_argument('--headless')
        options.add_argument('--verbose')
        options.add_argument("--log-path=chromedriver.log")
        options.add_argument('--disable-gpu')
        options.add_argument('--no-sandbox')
        options.binary_location = "/usr/bin/"
        
        driver = webdriver.Chrome(executable_path="/usr/lib/chromium-browser/chromedriver",chrome_options=options)
    
    elif browser == 'phantom':
        driver = webdriver.PhantomJS()

    return driver
