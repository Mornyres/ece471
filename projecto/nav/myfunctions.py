from selenium.webdriver.chrome.options import Options
# we need javascript support
from selenium import webdriver

# for parsing web pages
from bs4 import BeautifulSoup


def urlMaker(colorList,service):
    if (service == 'G'):
        # example url:
        # https://artsexperiments.withgoogle.com/artpalette/colors/8e8c4c-b77646-51657c
 
        url = "https://artsexperiments.withgoogle.com/artpalette/colors/"
        for col in colorList:
            url = url + (col)
            url = url + ("-")

        # removes last char: last color should not have a dash
        url=url[:-1]

 
    elif (service == 'M'):
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

def imageParse(parenturl,maxImg):

        print "Parsing: " + parenturl
        # from ://stackoverflow.com/questions/18408307/how-to-extract-and-download-all-images-from-a-website-using-beautifulsoup
        webpage = requests.get(parenturl)

        # extract server response as raw data

        webpage = webpage.content

        debugFile = open("rawpage.html", 'wb')
        debugFile.write(webpage.decode('cp1252'))

    	soup = BeautifulSoup(webpage, 'html.parser')


        # below works for most cases but not all
    	img_tags = soup.find_all('img')



        urls = [img['src'] for img in img_tags]
        
        # format for printing to a debug file (which we wound up using a lot)
        debugFile = open("soup.html", 'wb')
        debugFile.write(soup.prettify("utf-8"))


        # must also follow dividers
        

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

def javaParse(parenturl,maxImg):

        print "Parsing: " + parenturl
        

        #https://stackoverflow.com/questions/47596402/selenium-chrome-failed-to-start-exited-abnormally-error
        options = webdriver.ChromeOptions()
        options.add_argument('--disable-extensions')
        options.add_argument('--headless')
        options.add_argument('--disable-gpu')
        options.add_argument('--no-sandbox')
        options.binary_location = "/usr/bin/"

        driver = webdriver.Chrome(executable_path="/usr/lib/chromium-browser/chromedriver",chrome_options=options)
        driver.get(parenturl)

        # we can now directly manipulate the page
        # https://stackoverflow.com/questions/20986631/how-can-i-scroll-a-web-page-using-selenium-webdriver-in-python
        driver.execute_script("window.scrollTo(0),document.body.scrollHeight);")

        # https://selenium-python.readthedocs.io/locating-elements.html
        webpage=driver.find_element_by_class_name('results')

        debugFile = open("rawpage.html", 'wb')
        debugFile.write(webpage.decode('cp1252'))

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
		filename = "pics/pic" + str(index) + ".png"
		with open(filename, 'wb') as f:
                        if 'http' not in url:
                            url = '{}{}'.format(parenturl,url)
			response = requests.get(url)
        		f.write(response.content)
                        print"File " + filename + " written"

	return len(urls)
