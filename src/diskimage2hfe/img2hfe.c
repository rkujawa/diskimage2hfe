/*
 * diskimage2hfe (c) 2010 Radoslaw Kujawa
 *
 * Convert diskette images to HFE format on AmigaOS. 
 *
 * This program is mostly portable, should build on any 
 * UNIX or AmigaOS-like system.
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "hxc_floppy_emulator.h"
#include "internal_floppy.h"
#include "floppy_loader.h"
#include "licensetxt.h"

#include "hfe_file_writer.h"

int print_hxc_message(int MSGTYPE,char * chaine, ...) {
	va_list marker;
	va_start(marker, chaine);
	vprintf(chaine,marker);
	printf("\n");
	va_end(marker);
	return(0);
}

void print_banner(void) {
	printf("diskimage2hfe (c) 2010 Radoslaw Kujawa\n");
	printf("HxC Floppy Emulator Copyright (C) 2006-2009 Jean-Francois DEL NERO\n");
	printf("This program comes with ABSOLUTELY NO WARRANTY\n");
	printf("This is free software, and you are welcome to redistribute it\n");
	printf("Please read attached COPYING and COPYING_FULL files.\n\n");
}

void usage(char *myname) {
	printf("%s image.adf image.hfe\n", myname);
}

int main(int argc, char* argv[]) {

	HXCFLOPPYEMULATOR *flopemu;
	FLOPPY *thefloppydisk;
	int ret;
	
	print_banner();

	if( (argc != 3) ) {
		usage(argv[0]);
		return(0);
	}

	flopemu=(HXCFLOPPYEMULATOR*)malloc(sizeof(HXCFLOPPYEMULATOR));
	flopemu->hxc_printf=&print_hxc_message;
	initHxCFloppyEmulator(flopemu);

	thefloppydisk=(FLOPPY*)malloc(sizeof(FLOPPY));
	ret=floppy_load(flopemu,thefloppydisk,argv[1]);


	if(ret!=LOADER_NOERROR) {
		switch(ret) {
			case LOADER_UNSUPPORTEDFILE:
				printf("Load error: image file not yet supported!\n");
				break;
			case LOADER_FILECORRUPT:
				printf("Load error: file corrupted or read error.\n");
				break;
			case LOADER_ACCESSERROR:
				printf("Load error: file access error.\n");
				break;
			default:
				printf("Load error: %d\n",ret);
				break;
		}
	} else {
		write_HFE_file(flopemu,thefloppydisk,argv[2],-1);
		floppy_unload(flopemu,thefloppydisk);
	}
	free(thefloppydisk);
	free(flopemu);
	return 0;
}
