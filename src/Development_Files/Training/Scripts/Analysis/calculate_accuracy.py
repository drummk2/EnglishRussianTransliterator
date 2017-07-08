# Author: Kieron Drumm
# Date: April 5th 2017
# This script compares the transliterated that I created with my own system
# to the transliterated data created using Python's transliterate module in
# order to ascertain the accuracy of my system.

import sys

correct_transliterations = 0
test_data_length = 20000

with open(sys.argv[1]) as my_file, open(sys.argv[2]) as control_file:
    my_line = my_file.readline().rstrip()
    control_line = control_file.readline().rstrip()

    while my_line != '' or control_line != '':
        if my_line == control_line:
            correct_transliterations += 1

        my_line = my_file.readline().rstrip()
        control_line = control_file.readline().rstrip()

    print (correct_transliterations/test_data_length)
