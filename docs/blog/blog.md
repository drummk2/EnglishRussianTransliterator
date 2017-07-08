# Blog: Automatic Transliteration between English and Russian

**Kieron Drumm**

## Blog Entry No. 1 (February 2nd 2017)

This blog marks the beginning of my fourth year project. 
Over the few months I shall develop a system that automatically transliterates between English and Russian.

This week I carried out research on the technologies necessary for developing my fourth year project.
I installed python 2.7 along with the Scrapy framework (including any necessary dependencies) as it will be
required for any data scraping. I also downloaded python's nltk package. Lastly I ensured that git was set up on
my computer and that my gitlab repository was cloned onto that computer. This will allow me to work at home when
working in the labs is inconvenient.

I have already gathered some parallel data but it is nowhere near enough. As a result for the next few days I
will continue to gather parallel data using a data scraper of my own creation. This process will be detailed in my 
next blog.

## Blog Entry No. 2 (February 16th 2017)

Since I last wrote, I met with my supervisor Dr. Liu, and discussed all that I had done so far, and everything that
I planned to do. Some of the main things that I have done since my last entry are as follows.

Firstly, I began familiarising myself with the Moses system as it will be extremely useful in the transliteration
part of my project. After installing it on my system, I researched some of the main scripts that I believe will
be relevant to my project.

I also gathered far more parallel data. As it proved to be quite difficult to find pre-transliterated data online,
with the use of a Python package I have found a way of creating my own parallel data that I can later use to train
my system. Now all I need do is to gather large amount of Russian text in Cyrillic. I can then transliterate it to
effectively create my own parallel training data.

As of now I have gathered about 180,000 words in Cyrillic and have created a file containing their transliterated
equivalent. I have also cleaned both files with the use of Moses' 'clean-corpus-n.perl' script.

My next step will be to continue gathering data and familiarising myself with Moses. I also plan to begin developing
a very simple user interface for my system.

## Blog Entry No. 3 (February 20th 2017)

Over the weekend I spent some time familiarising myself further with a python graphics library called Tkinter. This
week I plan to continue developing the user interface.

I also wrote some small scripts in python to clean the data that I have gathered so far. While developing the UI I also
plan to gather and clean as much training data as possible.

Lastly I plan to organise a meeting with Dr. Liu to discuss my progress and to plan out what should be done for the next
week or two.

## Blog Entry No. 4 (March 3rd 2017)

Yesterday I met with Dr. Liu to discuss my progress. He was satisfied with the amount of data that I have gathered thus far.

Recently I have spent my time learning about the way in which transliteration differs from translation in relation to using
the Moses system. I also spent some time writing a short Python script to tokenize my data. I have now successfully tokenized
all four of my training files, roughly 1.2 million sentence pairs.

I now plan to begin developing both a language and transliteration model with Moses. I will also continue with my interface as
most of my time this week in relation to the interface was spent researching how to run external scripts from the interface.

## Blog Entry No. 5 (March 22nd 2017)

Tomorrow I plan to meet with Dr. Liu to discuss my progress.

Since my last entry I have constructed a language model with the use of Moses and IRSTLM. This model is for Russian words
that are written in Cyrillic.

I have also thought of and noted some small alterations that could be made to the user interface, I plan to implement these once
I have constructed a translation model, and as a result, a basic functioning system. I will then proceed to improve the user
interface along with the overall system.

## Blog Entry No. 6 (April 5th 2017)

Over the last two weeks I have been able to successfully train a working translation model. I also spent some time adding some code 
to my interface to allow me to run external scripts, as I mentioned in a previous entry. I plan to meet with Dr. Liu next week to 
discuss my progress.

I also wrote a small script to calculate the accuracy of my current system, I did so with the use of a file containing 20000 romanized
words. I first transliterated said words into cyrillic with my own system, then again with the Transliteration module in Python. Finally
I compared both files line by line in order to determine the accuracy of my system. It should be noted that the current accuracy is 0.9947.
(See code below used to determine accuracy):

```python
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
```

I now plan to improve my system, if possible. I also plan to zip up all of my code into an executable in order to make using my system far
easier.

## Blog Entry No. 7 (April 9th 2017)

Today I successfully intergrated my user interface with the rest of my system. The only major error that I encountered involved trying to call
Moses using Python's subprocess module. However, in order to fix this problem I simply created a shell script to be called by the UI instead,
this script in turn called Moses.

The code in the shell script was simple and as follows:
```python
../External_Tools/moses -f ../Data_Training/Trans_Model/train/model/moses.ini < $1 > $2
```

I now plan to make the system more efficient as there is a quite a long time between requesting a transliteration and receiving said
transliteration.

## Blog Entry No. 8 (April 23rd 2017)

Since I last wrote I spent some time training a new transliteration model as I had made some major changes to my folder structure.

I also spent some time toying with the idea of using a web page in order to demonstrate my application, however I eventually
decided against this as I do not have enough space on the DCU server to store my application.

I now plan to tune my new model, and also to find a way of making the application runnable as an exe.

## Blog Entry No 9. (April 29th 2017)

Since my last entry, I made a second attempt at putting my application along with a web page onto the DCU server, I was successful
in uploading to the server. Unfortunately due to some security issues on the server I am unable to run my application. So it would
seem that I will ultimately be demonstrating my application via my desktop GUI.

I also plan to meet with Dr. Liu soon in order to demonstrate the application to him.

## Blog Entry No. 10 (May 5th 2017)

I have successfully generated a runnable exe for my application. I have also been spending some time on adding an 'about' window
to the application describing said application, and a 'tutorial' window explaining how to use the application. I have also spent
some time adding a dropdown menu to the GUI to allow the user to access these 'about' and 'tutorial' windows.

I now plan to finish this work, then I will progress onto gathering some new test data, running some tests and ultimately trying
to make the application run faster.

## Blog Entry No. 11 (May 12th 2017)

Although I did spend some more time on the tutorial and about windows, I have also managed to successfully binarize my language
model with the use of the KENLM toolkit. This toolkit by default comes with Moses. I carried out this binarization using the
command: $kenlm/build_binary lm.arpa.cy lm.arpa.cy.binary. After making a small change to the moses.ini file to account for this
change, my system now loads the binary file rather than the original ARPA file, thus decreasing overall load time.

Over the next few days I plan to complete any documentation for this project, and if possible I plan to create a compact form
of the phrase table that I am currently using in order to decrease overall transliteration time even further.

I also plan to make any finishing touches to my GUI, and to carry out any tests that I feel should be done.

## Blog Entry No. 12 (May 14th 2017) 

Today I spent some time gathering a large list of English words to act as the test data for my second round of testing.

In order to cut the list down to the required size (20000 words), I wrote a script to randomly choose 20000 lines from
the large file. These randomly chosen words are then placed into a new file. Next I plan to test my system's accuracy
using this new file. As the nature of these words vary greatly from the nature of the data with which I trained my
system, I expect a noticable decrease in the overall accuracy of my system as given by my testing script.

## Blog Entry No. 13 (May 16th 2017)

After testing my systems accuracy using both my own script (calculate_accuracy.py), and moses bleu score script (multi_bleu.perl)
I have gotten the following results.

calculate_accuracy.py:

    - Training data and testing data were in the same domain - 0.9947
    - Training data and testing data were in different domains - 0.8731

multi_bleu.perl:

    - Training data and testing data were in the same domain - 0.9971
    - Training data and testing data were in different domains - 0.9802

I now plan to finish of my documentation, and to record a video walkthrough of my software

## Blog Entry No. 14 (May 21st 2017)

I recorded, edited and uploaded my video walkthrough. By the end of today I will also have written the last section of my technical
specification. I have also made some changes to the user interface in accordance with some feedback I received during user testing. 

As a result the following changes have been made:

    - The user can now initiate the transliteration process by pressing the enter key
    - The 'about' window has been removed and replace with a smaller piece of text in the main window
    - When the main window is closed, any child windows are also closed
    - Resized the tutorial window to make it easier to read through
    - Added a shortcut (ctrl-c) to clear both textboxes in the main window
    - Added a message to indicate that the transliteration process has ended

I will record another entry once I have completed all documentation.

## Blog Entry No. 15 (May 22nd 2017)

I have now completed both my technical specification and user manual. 

Also in order to make reviewing any original code I have placed any scripts that are not third party into a separate folder within 
my "src" folder called "My_Source_Code".

Lastly one or two small changes were made to my tutorial.txt file in order to add some more information to my tutorial.
