# Author: Kieron Drumm
# Date: February 15th 2017
# This script is used to scrape some Russian text from the 'klassika.ru' website.

import scrapy

class KlassikaSpider (scrapy.Spider):
    name = 'klassika_spider'
    start_urls = ['http://www.klassika.ru/proza/goncharov/oblomov.txt']

    def toCyrillic(self, cyrillic_names):
        for name in cyrillic_names:
	    print name.encode('utf-8')

    def parse(self, response):
        cyrillic_text = response.xpath('//body/text()').extract()
        self.toCyrillic(cyrillic_text)
