# Author: Kieron Drumm
# Date: February 20th 2017
# Code used to create and configure the user interface for this
# transliteration application.

from Tkinter import Button, DISABLED, E, END, Entry, Frame, Label, Menu, StringVar, Text, Tk, W
import tkFont
import tkMessageBox
import subprocess
import sys

tutorial_open = False

# Container class for the user interface
# ======================================
class UserInterface:
    def __init__(self, master):
        # Created so that it can be referenced outside of init
        self.master_window = master

        # Make it so that the enter key also starts the transliteration process
        # Also add the following control commands:
        # Ctrl-c -> clear both textboxes
        self.master_window.bind('<Return>', self.transliterate)
        self.master_window.bind('<Control-c>', self.clear_boxes)

        # Set the default response for when this window is closed via the
        # window manager
        master.protocol('WM_DELETE_WINDOW', self.close_window)

        # Set the default fonts
        self.default_font = tkFont.Font(family = 'Arial', size = 12)

        # Set the default background colour
        self.default_colour = '#ffe8c6'

        # Set the dimensions, color and title of the parent window
        master.minsize(width = 600, height = 300)
        master.resizable(width = False, height = False)
        master.title('English to Russian Transliterator')
        master.configure(bg = self.default_colour)

        # Add a dropdown menu
        self.menu = Menu(master, tearoff = 0)
        master.configure(menu = self.menu)

        # Add a dropdown submenu
        self.help_submenu = Menu(self.menu, tearoff = 0)
        self.help_submenu.add_command(label = 'Tutorial', command = self.show_tutorial)
        self.menu.add_cascade(label = 'Help', menu = self.help_submenu)

        # Frame to encapsulate the main header
        # ====================================
        self.header_area = Frame(master)
        self.header_area.configure(bg = self.default_colour)
        self.header_area.grid(row = 0, pady = 10)

        # Create the main header
        self.main_header = Label(self.header_area, text = 'English to Russian Transliterator', font = ('Arial', 18))
        self.main_header.configure(bg = self.default_colour)
        self.main_header.grid(row = 0, padx = (80, 0))

        # Frame to encapsulate the text entry area of the GUI
        # ===================================================
        self.entry_area = Frame(master)
        self.entry_area.configure(bg = self.default_colour)
        self.entry_area.grid(row = 1, pady = (20, 10))

        # Initialise and position widgets
        # English label
        self.english_roman_label = Label(self.entry_area, text = 'Enter English Here: ', font = self.default_font, anchor = W)
        self.english_roman_label.configure(bg = self.default_colour)
        self.english_roman_label.grid(row = 1, sticky = E, padx = 5, pady = 5)

        # English textbox
        self.english_roman_entry = Entry(self.entry_area, width = 40, borderwidth = 1)
        self.english_roman_entry.grid(row = 1, column = 1, sticky = W, padx = 5, pady = 5)

        # Russian label
        self.russian_cyrillic_label = Label(self.entry_area, text = 'Russian Transliteration: ', font = self.default_font, anchor = W)
        self.russian_cyrillic_label.configure(bg = self.default_colour)
        self.russian_cyrillic_label.grid(row = 2, sticky = E, padx = 5, pady = 5)

        # Russian textbox
        self.russian_cyrillic_entry = Entry(self.entry_area, width = 40, borderwidth = 1)
        self.russian_cyrillic_entry.grid(row = 2, column = 1, sticky = W, padx = 5, pady = 5)

        # Frame to encapsulate any buttons
        # ================================
        self.button_section = Frame(master)
        self.button_section.configure(bg = self.default_colour)
        self.button_section.grid(row = 3, column = 0, pady = 10, padx = (40, 0))

        # Button to initiate transliteration process
        self.transliterate_button = Button(self.button_section, text = 'Transliterate', font = self.default_font)
        self.transliterate_button.configure(bg = 'white', borderwidth = 1)
        self.transliterate_button.bind('<Button-1>', self.transliterate)
        self.transliterate_button.grid(row = 4, column = 0, sticky = E, pady = (10,0), padx = 10)

        # Button to clear all text boxes when clicked
        self.clear_button = Button(self.button_section, text = 'Clear', font = self.default_font)
        self.clear_button.configure(bg = 'white', borderwidth = 1)
        self.clear_button.bind('<Button-1>', self.clear_boxes)
        self.clear_button.grid(row = 4, column = 1, sticky = E, pady = (10,0), padx = 10)

        # Frame in which the progress label will be placed
        # ================================================
        self.progress_section = Frame(master)
        self.progress_section.configure(bg = self.default_colour)
        self.progress_section.grid(row = 4, column = 0, pady = 10, padx = (80, 0))

        # Label to display any progress notices to the user
        self.current_progress = StringVar()
        self.progress_label = Label(self.progress_section, textvariable = self.current_progress, font = self.default_font)
        self.progress_label.configure(bg = self.default_colour)
        self.progress_label.grid(row = 1, column = 1, padx = 5, pady = 5)

        # Frame to encapsulate the masthead
        # =================================
        self.masthead_area = Frame(master)
        self.masthead_area.configure(bg = self.default_colour)
        self.masthead_area.grid(row = 5, column = 0, pady = 10, padx = 10)

        # Labels to display the current version number and name of the developer
        self.version_number = Label(self.masthead_area, text = 'Version 1.0', font = ('Arial', '10'))
        self.version_number.configure(bg = self.default_colour)
        self.version_number.grid(row = 0, column = 0, padx = (0, 10))
        self.developer_name = Label(self.masthead_area, text = 'Author: Kieron Drumm (kieron.drumm2@mail.dcu.ie)', font = ('Arial', '10'))
        self.developer_name.configure(bg = self.default_colour)
        self.developer_name.grid(row = 0, column = 1, padx = (10, 160))

    # Function to transliterate English to Russian once
    # the 'transliterate button is pressed
    def transliterate(self, event):
        if(self.english_roman_entry.get() != ''):
            self.current_progress.set('Transliterating...')
            self.master_window.update_idletasks()
            self.createInputFile()
            process = subprocess.Popen(['sh', './run_moses', 'input.ro', 'output.cy'], stdout = subprocess.PIPE)
            command_output, errors = process.communicate()
            self.progress_label.configure(text = '')
            output_string = self.getOutput()
            self.russian_cyrillic_entry.delete(0, END)
            self.russian_cyrillic_entry.insert(0, output_string)
            self.current_progress.set('Transliteration Complete')
        else:
            self.current_progress.set('Must provide an English sentence')

    # Function to take any input from the English textbox
    # This input will be placed into a file that can then
    # be given to Moses at runtime
    def createInputFile(self):
        input_file = open('input.ro', 'w+')
        # Note that the string is formatted for Moses(each word on its own line
        # and split into its individual characters)
        input_words = self.english_roman_entry.get().split(' ')
        for word in input_words:
            input_file.write(word.replace('', ' ').lower() + '\n')
        input_file.close()

    # Function to take Moses' output from output.cy and
    # place it into the Russian textbox
    def getOutput(self):
        output_file = open('output.cy', 'r+')
        output_words = output_file.readlines()
        output_string = ''
        for word in output_words:
            output_string += word.replace(' ', '').replace('\n', '') + ' '
        output_file.close()
        return output_string

    # Function to clear all text boxes once the 'clear'
    # button is pressed
    def clear_boxes(self, event):
        self.english_roman_entry.delete(0, END)
        self.russian_cyrillic_entry.delete(0, END)
        self.current_progress.set('')

    # Function to show a tutorial for the application when the
    # tutorial button is clicked
    def show_tutorial(self):
        global tutorial_open
        if not tutorial_open:
            tutorial = Tk()
            self.tutorial_window = TutorialWindow(tutorial)
            tutorial_open = True
            tutorial.mainloop()

    # Function to handle the window being closed
    def close_window(self):
        global tutorial_open
        if tkMessageBox.askyesno('Quit', 'Are you sure you want to quit?'):
            self.master_window.destroy()
            if tutorial_open:
                self.tutorial_window.master_window.destroy()

# Container class for the tutorial section
# ========================================
class TutorialWindow:
    def __init__(self, master):
        # Created so that it can be referenced outside of init
        self.master_window = master

        # Set the default response for when this window is closed via the
        # window manager
        master.protocol('WM_DELETE_WINDOW', self.close_window)

         # Set the default fonts
        self.default_font = tkFont.Font(family = 'Arial', size = 12)

        # Set the default background colour
        self.default_colour = '#ffe8c6'

        # Set the dimensions, color and title of the parent window
        master.minsize(width = 600, height = 500)
        master.resizable(width = False, height = False)
        master.title('Tutorial')
        master.configure(bg = self.default_colour)

        # Add a dropdown menu
        self.menu = Menu(master, tearoff = 0)
        master.configure(menu = self.menu)

        # Add a dropdown submenu
        self.help_submenu = Menu(self.menu, tearoff = 0)
        self.help_submenu.add_command(label = 'Tutorial', command = '')
        self.menu.add_cascade(label = 'Help', menu = self.help_submenu)

        # Tutorial
        # The tutorial is held in an external file and read in when necessary
        with open('tutorial.txt') as tutorial_file:
            self.tutorial_info = tutorial_file.readlines()
            self.explanation = Text(master, height = 40, width = 86)
            self.explanation.grid(row = 1, padx = 0, pady = 0)
            for line in self.tutorial_info:
                self.explanation.insert(END, line + '\n')
            self.explanation.configure(state = DISABLED)

    # Function to handle the window being closed
    def close_window(self):
        global tutorial_open
        tutorial_open = False
        self.master_window.destroy()

# Initialise main window and add main frame to said window.
# =========================================================
if __name__ == "__main__":
    parent_window = Tk()
    user_interface = UserInterface(parent_window)
    parent_window.mainloop()
