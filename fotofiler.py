# Author:  D.J. Platt
# Date:    Mar 2021
# Purpose: Copy photos to YYYY/MM/DD directory structure
#
import os
import shutil
from tkinter import *
import exifread
from tkinter import filedialog
from tkinter import scrolledtext 
#
########################################################################
#
def getYear(dstring):
    return (dstring[0:4])

def getMonth(dstring):
    return (dstring[5:7])

def getDay(dstring):
    return (dstring[8:10])

def getOS():
    return os.name

def extension(x):
    ext=x.split(".")
    return (ext[1])

def isPicture(lname):
    extList = ['jpg', 'JPG','jpeg','ARW','NEF']
    for i in extList:
       if extension(lname) == i:
          return True

def findSource():
    img_dir = filedialog.askdirectory()
    dirName.set(img_dir)
    os.chdir(img_dir)
   
    
def findDestRoot():
    img_dir = filedialog.askdirectory()
    destName.set(img_dir)
   

def process():
    if getOS() == "posix":
        slash = "/"
    else:
        slash = "\\"


    src = srcDirText.get()
    dest = destDirText.get()
    dayDir = ''
    #
    #  iterate over all files in the source directory
    #
    for file in os.listdir(src):
        fname = src + slash + file
    #
    #  if it is not a directory and is a picturefile ...
    #
        if os.path.isfile(fname):
            if isPicture(file):
                f = open(file, 'rb')
                tags = exifread.process_file(f)
                dto = tags.get('EXIF DateTimeOriginal')                  
    #
    #  determine what the names of the destination directories should be
    #
                yearDir = dest + slash + (getYear(str(dto)))
                monthDir = yearDir + slash + (getMonth(str(dto)))
                dayDir = monthDir + slash + (getDay(str(dto)))
    #
    #  check to see if the year/month/day directories exist and create them if not
    #
                isYear = os.path.isdir(yearDir)            
                if isYear:
                    pass
                else:
                    textArea.insert('insert','Creating ' + yearDir + '\n')
                    os.mkdir(yearDir)
                    
                isMonth = os.path.isdir(monthDir)    
                if isMonth:
                    pass
                else:
                    textArea.insert('insert','Creating ' + monthDir + '\n')
                    os.mkdir(monthDir)
                   
                isDay = os.path.isdir(dayDir)
                if isDay:
                    pass
                else:
                    textArea.insert('insert','Creating ' + dayDir + '\n')
                    os.mkdir(dayDir)
    #
    #  don't copy the file if it exists already
    #
                destfile = dayDir + slash + file
                if os.path.isfile(destfile):
                     textArea.insert('insert','Skipping ' +  destfile + '\n')
                else:
                    shutil.copyfile(fname,destfile)
                    textArea.insert('insert','Copying ' +  destfile + '\n')

                f.close()

    textArea.insert('insert','Copy Complete \n')
###############################################################################
#
# Create the root window
#
window = Tk()
window.title('FotoFiler')
window.geometry("500x700")
window.config(background = "white")
#  
# screen element definition
#
label_file_explorer = Label(window,text = "FotoFiler",font=("arial italic", 18),width = 26, height = 3, fg = "blue",bg="yellow") 
button_src = Button(window, text = "Select Source Directory", command = findSource)
button_dest = Button(window, text = "Select Destination Directory", command = findDestRoot)
dirName = StringVar()
srcDirText = Entry(window, width = 50, textvariable = dirName)
destName = StringVar()
destDirText = Entry(window, width = 50, textvariable = destName)
textArea = scrolledtext.ScrolledText(window, width = 70, height = 15,font = ("Helvetica",8)) 
button_copy = Button(window, text = "Copy",command = process) 
#  
# place screen elements
#
label_file_explorer.place(x = 50, y = 20)
button_src.place(x=150,y=150)
srcDirText.place(x=50,y=200)
button_dest.place(x=135, y=250)
destDirText.place(x=50,y=300)
textArea.place(x=50,y=350)
button_copy.place(x=300,y=600)  
#
# Let the window wait for any events
#
window.mainloop()
