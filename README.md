# fotoFile
###################
An attempt to port a python script, which organizes files by reading exifdata and creating a year/month/day
directory structure, to C++ 

2021.03.14  started coding.  reading jpeg exif data and parsing out date information.

2021.03.26 version working.  lots of transfers between string and char because some of the code I
pillaged uses strings and some use char.  Will clean this up later.

Next steps:  port to windows
             add screen instead of invoking from command line
             clean up strings/chars
             working with jpeg files now.  add other formats
             add python script to this repository
