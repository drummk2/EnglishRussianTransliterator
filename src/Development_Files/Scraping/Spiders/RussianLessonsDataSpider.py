# Author: Kieron Drumm
# Date: February 15th 2017
# This script uses scrapy to scrape russian text from multiple essays written in Russian.
# The data is taken from the website 'russianlesson.net'.

import scrapy

class RussianLessonsDataSpider (scrapy.Spider):
    name = 'russian_lesson_data_spider'
    start_urls = ['http://www.russianlessons.net/articles/mynameismasha.php?lang=russian',
                  'http://www.russianlessons.net/articles/russianlanguage.php?lang=russian',
                  'http://www.russianlessons.net/articles/russianwinter.php?lang=russian',
                  'http://www.russianlessons.net/articles/russianwomen.php?lang=russian',
                  'http://www.russianlessons.net/articles/russianmen.php?lang=russian',
                  'http://www.russianlessons.net/articles/yurigagarin.php?lang=russian',
                  'http://www.russianlessons.net/articles/russiansocialism.php?lang=russian',
                  'http://www.russianlessons.net/articles/russianfood.php?lang=russian',
                  'http://www.russianlessons.net/articles/seaside.php?lang=russian',
                  'http://www.russianlessons.net/articles/moscow.php?lang=russian',
                  'http://www.russianlessons.net/articles/russiansport.php?lang=russian',
                  'http://www.russianlessons.net/articles/transsiberianrailway.php?lang=russian',
                  'http://www.russianlessons.net/articles/relationships.php',
                  'http://www.russianlessons.net/articles/russianhistory.php?lang=russian',
                  'http://www.russianlessons.net/articles/globalisation.php?lang=russian',
                  'http://www.russianlessons.net/articles/russianmilitary.php?lang=russian',
                  'http://www.russianlessons.net/articles/saintpetersburg.php?lang=russian',
                  'http://www.russianlessons.net/articles/russianweddings.php?lang=russian',
                  'http://www.russianlessons.net/articles/trains.php?lang=russian', 
                  'http://www.russianlessons.net/ebooks/show/1/1/1/russian',
                  'http://www.russianlessons.net/ebooks/show/1/1/2/russian',
                  'http://www.russianlessons.net/ebooks/show/1/1/3/russian',
                  'http://www.russianlessons.net/ebooks/show/1/1/4/russian',
                  'http://www.russianlessons.net/ebooks/show/1/1/5/russian',
                  'http://www.russianlessons.net/ebooks/show/1/1/6/russian',
                  'http://www.russianlessons.net/ebooks/show/1/1/7/russian']

    def toCyrillic(self, cyrillic_names):
        for name in cyrillic_names:
	    print name.encode('utf-8')

    def parse(self, response):
        for text_block in response.css('.main_document'):
            cyrillic_text = text_block.xpath('.//article/p/text()').extract()
            self.toCyrillic(cyrillic_text)

