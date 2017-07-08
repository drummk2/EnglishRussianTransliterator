# Author: Kieron Drumm
# Date: May 14th 2017
# This script can be used to generate test data from a large in the
# event that one wishes to select random lines from a file in order
# to create a brand new randomised test file

import random, sys

word_count = 0
with open(sys.argv[1]) as large_file, open('test_data.txt', 'w+') as test_data:
    large_file_list = large_file.readlines()
    test_data_list = list()
    while word_count < 20000:
        random_word = large_file_list[random.randint(1, len(large_file_list))]
        test_data_list.append(random_word)
        word_count += 1
    test_data.writelines(test_data_list)
