# Author: Kieron Drumm
# Data: February 15th 2017
# This script can be used to convert data in English/Russian to it's transliterated form in the Russian/Roman alphabet,
# It will be used not to carry out my actual tranlsiteration, but to create parallel data.

import sys
from transliterate import translit

with open(sys.argv[1], 'r') as file:
    lines = file.read()

print(translit(lines, "ru"))
