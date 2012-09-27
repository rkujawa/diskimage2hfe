#include "hxc_floppy_emulator.h"
#include "internal_floppy.h"
#include "floppy_loader.h"
#include "usb_floppyemulator/usb_hxcfloppyemulator.h"
#include "amiga_api.h"
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

void strlwr(char *string) {
	int i;

	i=0;
	while (string[i]) {
		string[i] = tolower(string[i]);
		i++;
	}
}


int getlistoffile(unsigned char * directorypath,unsigned char *** filelist)
{
	return 0;/*numberoffile;*/
}


char * getcurrentdirectory(char *currentdirectory,int buffersize)
{
	return 0;
}


int loaddiskplugins(HXCFLOPPYEMULATOR* floppycontext,FLOPPY * newfloppy,char *pluginpath)
{
	return 0;
}

long find_first_file(char *folder,char *file,filefoundinfo* fileinfo)
{
	return -1;
}

long find_next_file(long handleff,char *folder,char *file,filefoundinfo* fileinfo)
{
	return 0;
}

long find_close(long handle)
{
	return 0;
}

char * strupper(char * str)
{
	int i;
	
	i=0;
	while(str[i]) {
		if(str[i]>='a' && str[i]<='z') {
			str[i]=str[i]+('A'-'a');
		}
		i++;
	}

	return str;
}


char * strlower(char * str) {
	int i;
	
	i=0;
	while(str[i]) {
	
		if(str[i]>='A' && str[i]<='Z') {
			str[i]=str[i]+('a'-'A');
		}
		i++;
	}
	return str;
}
