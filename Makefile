CXX=g++
CXXFLAGS=-O2 -pedantic -Wall -Wextra -ansi -std=c++11

all: fotoFile

exif.o: exif.cpp
	$(CXX) $(CXXFLAGS) -c exif.cpp

fotoFile: exif.o fotoFile.cpp
	$(CXX) $(CXXFLAGS) -o fotoFile exif.o fotoFile.cpp

clean:
	rm -f *.o fotoFile fotoFile.exe
