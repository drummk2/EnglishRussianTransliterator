# Author: Kieron Drumm
# Date: March 1st 2017
# This script splits individual words into their respective characters.
# As a result those characters can be treated as words in a sentence when 
# training an mt system for the purpose of transliteration.

import sys

with open(sys.argv[1]) as untokenized_file, open('tokenized_file.txt', 'w+b') as tokenized_file:
    untokenized_text = [word.strip() for word in untokenized_file.readlines()]
    tokenized_text = [word.replace('', ' ') for word in untokenized_text]
    for word in tokenized_text:
        word_with_newline = word + '\n'
        tokenized_file.write(word_with_newline.encode('utf-8'))
