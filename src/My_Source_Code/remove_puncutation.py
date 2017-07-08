# Author: Kieron Drumm
# Date: February 18th 2017
# Use this script to remove any punctuation from a file. This will be used
# to clean any data that is scraped from the internet.

import os.path
import re
import sys

with open(sys.argv[1]) as uncleaned_file, open('output.txt', 'w+') as cleaned_file:
    uncleaned_text = uncleaned_file.read()
    if os.path.splitext(uncleaned_file.name)[1] == '.cy':
        cleaned_text = re.sub('[.,!:;?"A-Za-z]', ' ', uncleaned_text)
    else:
        cleaned_text = re.sub('[^A-Za-z]', ' ', uncleaned_text)
    cleaned_file.write(cleaned_text)
