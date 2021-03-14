#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <locale.h>
#include "exif.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/stat.h>


using namespace std;


string getYear ( string dateString )
{
	return dateString.substr(0,4);
}

string getMonth ( string dateString )
{
	return dateString.substr(5,2);
}

string getDay ( string dateString )
{
	return dateString.substr(8,2);
}

void display ( string printString )
{
    cout << printString;
    cout << "\n";
}


bool directoryExists(const char *path){
    struct stat stats;

    stat(path, &stats);
    // Check for file existence
    if (S_ISDIR(stats.st_mode))
        return true;

    return false;
}

bool fileExists(const char *path){
    // Try to open file
    FILE *fptr = fopen(path, "r");

    // If file does not exist
    if (fptr == NULL)
        return false;

    // File exists hence close file and return true.
    fclose(fptr);
    return true;
}

static void list_directory(const char *dirname)
{
	/* Scan files in directory */
	struct dirent **files;
	int n = scandir(dirname, &files, NULL, alphasort);
	if (n < 0) {
		fprintf(stderr,
			"Cannot open %s (%s)\n", dirname, strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* Loop through file names */
	for (int i = 0; i < n; i++) {
		/* Get pointer to file entry */
		struct dirent *ent = files[i];

		/* Output file name */
		switch (ent->d_type) {
		case DT_REG:
			printf("%s\n", ent->d_name);
			break;

		case DT_DIR:
			printf("%s/\n", ent->d_name);
			break;

		case DT_LNK:
			printf("%s@\n", ent->d_name);
			break;

		default:
			printf("%s*\n", ent->d_name);
		}

	}

	/* Release file names */
	for (int i = 0; i < n; i++) {
		free(files[i]);
	}
	free(files);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: demo <JPEG file>\n");
    return -1;
  }

  char source [] = "/home/david/Pictures/IMG_6394.jpg";
  char target [] = "/home/david/Pictures";




  // Read the JPEG file into a buffer
  FILE *fp = fopen(argv[1], "rb");
  if (!fp) {
    printf("Can't open file.\n");
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



  /* Dump EXIF information
  printf("Camera make          : %s\n", result.Make.c_str());
  printf("Camera model         : %s\n", result.Model.c_str());
  printf("Software             : %s\n", result.Software.c_str());
  printf("Bits per sample      : %d\n", result.BitsPerSample);
  printf("Image width          : %d\n", result.ImageWidth);
  printf("Image height         : %d\n", result.ImageHeight);
  printf("Image description    : %s\n", result.ImageDescription.c_str());
  printf("Image orientation    : %d\n", result.Orientation);
  printf("Image copyright      : %s\n", result.Copyright.c_str());
  printf("Image date/time      : %s\n", result.DateTime.c_str());
  printf("Original date/time   : %s\n", result.DateTimeOriginal.c_str());*/

  string odt = result.DateTimeOriginal;
  display(odt);
  display(getYear(odt));
  display(getMonth(odt));
  display(getDay(odt));

  /*
  printf("Digitize date/time   : %s\n", result.DateTimeDigitized.c_str());
  printf("Subsecond time       : %s\n", result.SubSecTimeOriginal.c_str());
  printf("Exposure time        : 1/%d s\n", (unsigned)(1.0 / result.ExposureTime));
  printf("F-stop               : f/%.1f\n", result.FNumber);
  printf("Exposure program     : %d\n", result.ExposureProgram);
  printf("ISO speed            : %d\n", result.ISOSpeedRatings);
  printf("Subject distance     : %f m\n", result.SubjectDistance);
  printf("Exposure bias        : %f EV\n", result.ExposureBiasValue);
  printf("Flash used?          : %d\n", result.Flash);
  printf("Flash returned light : %d\n", result.FlashReturnedLight);
  printf("Flash mode           : %d\n", result.FlashMode);
  printf("Metering mode        : %d\n", result.MeteringMode);
  printf("Lens focal length    : %f mm\n", result.FocalLength);
  printf("35mm focal length    : %u mm\n", result.FocalLengthIn35mm);
  printf("GPS Latitude         : %f deg (%f deg, %f min, %f sec %c)\n",
         result.GeoLocation.Latitude, result.GeoLocation.LatComponents.degrees,
         result.GeoLocation.LatComponents.minutes,
         result.GeoLocation.LatComponents.seconds,
         result.GeoLocation.LatComponents.direction);
  printf("GPS Longitude        : %f deg (%f deg, %f min, %f sec %c)\n",
         result.GeoLocation.Longitude, result.GeoLocation.LonComponents.degrees,
         result.GeoLocation.LonComponents.minutes,
         result.GeoLocation.LonComponents.seconds,
         result.GeoLocation.LonComponents.direction);
  printf("GPS Altitude         : %f m\n", result.GeoLocation.Altitude);
  printf("GPS Precision (DOP)  : %f\n", result.GeoLocation.DOP);
  printf("Lens min focal length: %f mm\n", result.LensInfo.FocalLengthMin);
  printf("Lens max focal length: %f mm\n", result.LensInfo.FocalLengthMax);
  printf("Lens f-stop min      : f/%.1f\n", result.LensInfo.FStopMin);
  printf("Lens f-stop max      : f/%.1f\n", result.LensInfo.FStopMax);
  printf("Lens make            : %s\n", result.LensInfo.Make.c_str());
  printf("Lens model           : %s\n", result.LensInfo.Model.c_str());
  printf("Focal plane XRes     : %f\n", result.LensInfo.FocalPlaneXResolution);
  printf("Focal plane YRes     : %f\n", result.LensInfo.FocalPlaneYResolution);*/

  if (directoryExists(target))  {
        printf("Directory exists at path '%s'\n", target);
  }
  else {
        printf("Directory does not exist at path '%s'\n", target);
  }

  if (fileExists(source))  {
        printf("File exists at path '%s'\n", source);
  }
  else {
        printf("File does not exist at path '%s'\n", source);
  }

  return 0;
}
