# Author: Kieron Drumm
# Date: February 1st 2017
# This script is used scrape a collection of names along with their pronunciations from
# a site name masterrussian.com

import scrapy

class MasterRussianDataSpider (scrapy.Spider):
    name = 'master_russian_data_spider'
    start_urls = ['http://masterrussian.com/aa031001a.shtml',
                  'http://masterrussian.com/aa031001b.shtml',
                  'http://masterrussian.com/aa031001c.shtml',
                  'http://masterrussian.com/aa031001d.shtml',
                  'http://masterrussian.com/aa031701a.shtml',
                  'http://masterrussian.com/aa031701b.shtml',
                  'http://masterrussian.com/aa031701c.shtml',
                  'http://masterrussian.com/aa031701d.shtml']

    def toCyrillic(self, cyrillic_names):
        for name in cyrillic_names:
	    print name.encode('utf-8')

    def printList(self, list):
        for element in list:
	    print element

    def parse(self, response):
        for name_set in response.css('table.topwords.cntr'):
            cyrillic_names = name_set.xpath('.//tbody/tr[not(contains(@class, "rowTop"))]/td[1]/text()').extract()
            self.toCyrillic(cyrillic_names)
            roman_names = name_set.xpath('.//tbody/tr[not(contains(@class, "rowTop"))]/td[3]/text()').extract()
            self.printList(roman_names)
