#include <stdio.h>
#include "exif.h"
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/stat.h>
#include <filesystem>
#include <dirent.h> 
#include <fstream>
#include <sstream>
#include <vector>

const static int BUF_SIZE = 4096;

using namespace std;

string getYear ( string dateString ){
   return dateString.substr(0,4);
}

string getMonth ( string dateString ){
   return dateString.substr(5,2);
}

string getDay ( string dateString ){
   return dateString.substr(8,2);
}

void display ( string printString ){
    cout << printString;
    cout << endl;
}

bool fileExists(string path){
    fstream localFile;
    localFile.open(path, ios::in);
    if (localFile){
        return true;
    }
    else{
        return false;
    }
    localFile.close();
 }

bool isPicture(string fn){
   string extensions[] = {"jpg","JPG","jpeg","JPEG"};
   for ( int i = 0; i < 4; i++ ) {
      if(fn.substr(fn.find_last_of(".") + 1) == extensions[i]) {
        return true;
      }    
    } 
    return false;
}

string chartoString(char * lchar){
   string lstring;
   stringstream ss;
   ss << lchar;
   ss  >> lstring;
   return lstring;   
}

void copyPicture(char * instring,char * outstring){
  ifstream  src(instring, ios::binary);
  ofstream  dst(outstring, ios::binary);
  dst << src.rdbuf();
}

vector<string> listDir(const char * path) {
   struct dirent *entry;
   vector <string> dirVect;
   DIR *dir = opendir(path); 
   if (dir == NULL) {
      cout << "Failed to open directory " << path <<endl;
      return dirVect;
   }

   while ((entry = readdir(dir)) != NULL) {
      string lstring = chartoString(entry->d_name);
      if (isPicture(lstring)){
          dirVect.push_back(lstring);
      }
   }

   closedir(dir);
   return dirVect; 
}

void checkDirectory(string lstring){
  char wChar [100];
  if (!fileExists(lstring))  {  
    cout << "Creating directory at path " << lstring << endl;
    strcpy(wChar, lstring.c_str());
    cout << wChar << '\n';
    if (mkdir(wChar,0775) != 0) {
      cout << "unable to create directory " << lstring << endl;
      cerr << "Error :  " << strerror(errno) << endl;
    }
  }
}


//########################################################################################################
int main(int argc, char *argv[]) {

  if (argc != 2) {
    printf("Usage: fotoFile sourceDirectory\n");
    return -1;
  }
 
  char target [100] = "/home/david/Pictures";
  string Target = "/home/david/Pictures";

  char source  [100];
  strcpy(source, argv[1]);
  string Source;
  Source = chartoString(argv[1]);

  string file;
  char cFile[100];
  string slash = "/";
 
  char wChar [100];
  
  vector <string> dirVect;
  dirVect = listDir(source);
  for (int i = 0; i < int(dirVect.size()); i++){
  
    Target = chartoString(argv[1]);
    strcpy(target, argv[1]);
    file = Source;  
    
  // Read the JPEG file into a buffer
    file.append(slash);
    file.append(dirVect[i]);
    strcpy(wChar, file.c_str());
    FILE *fp = fopen(wChar, "rb");
    if (!fp) {
      cout << "Can't open file" << wChar << endl;
      return -1;
    }
  
    fseek(fp, 0, SEEK_END);
    unsigned long fsize = ftell(fp);
    rewind(fp);
    unsigned char *buf = new unsigned char[fsize];
    if (fread(buf, 1, fsize, fp) != fsize) {
      printf("Can't read file.\n");
      delete[] buf;
      return -2;
    }
    fclose(fp);

    // Parse EXIF
    easyexif::EXIFInfo result;
    int code = result.parseFrom(buf, fsize);
    delete[] buf;
    if (code) {
      printf("Error parsing EXIF: code %d\n", code);
      return -3;
    }

    string odt = result.DateTimeOriginal;
    string dirYear = getYear(odt);
    string dirMonth = getMonth(odt);
    string dirDay = getDay(odt);
 
//  check to ensure that the year directory exists  
    Target.append(slash);
    Target.append(dirYear);
    checkDirectory(Target);

//  check to ensure that the month directory exists  
    Target.append(slash);
    Target.append(dirMonth);
    checkDirectory(Target);
  
 //  check to ensure that the day directory exists  
    Target.append(slash);
    Target.append(dirDay);
    checkDirectory(Target);

    Target.append(slash);
    Target.append(dirVect[i]);
  
    if (!fileExists(Target))  {
      strcpy(cFile,file.c_str());
      strcpy(target,Target.c_str());
      cout << "Copying " << cFile << " to " << target << endl;
      copyPicture(cFile,target);
    }
  }
  return 0;
  
}