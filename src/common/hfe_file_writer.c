/*
//
// Copyright (C) 2006, 2007, 2008, 2009 Jean-François DEL NERO
//
// This file is part of HxCFloppyEmulator.
//
// HxCFloppyEmulator may be used and distributed without restriction provided
// that this copyright statement is not removed from the file and that any
// derivative work contains the original copyright notice and the associated
// disclaimer.
//
// HxCFloppyEmulator is free software; you can redistribute it
// and/or modify  it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// HxCFloppyEmulator is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//   See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with HxCFloppyEmulator; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
*/

#define DIRECTWRITE 1

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "hxc_floppy_emulator.h"
#include "internal_floppy.h"

#include "hfe_file_writer.h"

unsigned char bit_inverter[]=
{
        0x00,0x80,0x40,0xC0,0x20,0xA0,0x60,0xE0,
        0x10,0x90,0x50,0xD0,0x30,0xB0,0x70,0xF0,
        0x08,0x88,0x48,0xC8,0x28,0xA8,0x68,0xE8,
        0x18,0x98,0x58,0xD8,0x38,0xB8,0x78,0xF8,
        0x04,0x84,0x44,0xC4,0x24,0xA4,0x64,0xE4,
        0x14,0x94,0x54,0xD4,0x34,0xB4,0x74,0xF4,
        0x0C,0x8C,0x4C,0xCC,0x2C,0xAC,0x6C,0xEC,
        0x1C,0x9C,0x5C,0xDC,0x3C,0xBC,0x7C,0xFC,
        0x02,0x82,0x42,0xC2,0x22,0xA2,0x62,0xE2,
        0x12,0x92,0x52,0xD2,0x32,0xB2,0x72,0xF2,
        0x0A,0x8A,0x4A,0xCA,0x2A,0xAA,0x6A,0xEA,
        0x1A,0x9A,0x5A,0xDA,0x3A,0xBA,0x7A,0xFA,
        0x06,0x86,0x46,0xC6,0x26,0xA6,0x66,0xE6,
        0x16,0x96,0x56,0xD6,0x36,0xB6,0x76,0xF6,
        0x0E,0x8E,0x4E,0xCE,0x2E,0xAE,0x6E,0xEE,
        0x1E,0x9E,0x5E,0xDE,0x3E,0xBE,0x7E,0xFE,
        0x01,0x81,0x41,0xC1,0x21,0xA1,0x61,0xE1,
        0x11,0x91,0x51,0xD1,0x31,0xB1,0x71,0xF1,
        0x09,0x89,0x49,0xC9,0x29,0xA9,0x69,0xE9,
        0x19,0x99,0x59,0xD9,0x39,0xB9,0x79,0xF9,
        0x05,0x85,0x45,0xC5,0x25,0xA5,0x65,0xE5,
        0x15,0x95,0x55,0xD5,0x35,0xB5,0x75,0xF5,
        0x0D,0x8D,0x4D,0xCD,0x2D,0xAD,0x6D,0xED,
        0x1D,0x9D,0x5D,0xDD,0x3D,0xBD,0x7D,0xFD,
        0x03,0x83,0x43,0xC3,0x23,0xA3,0x63,0xE3,
        0x13,0x93,0x53,0xD3,0x33,0xB3,0x73,0xF3,
        0x0B,0x8B,0x4B,0xCB,0x2B,0xAB,0x6B,0xEB,
        0x1B,0x9B,0x5B,0xDB,0x3B,0xBB,0x7B,0xFB,
        0x07,0x87,0x47,0xC7,0x27,0xA7,0x67,0xE7,
        0x17,0x97,0x57,0xD7,0x37,0xB7,0x77,0xF7,
        0x0F,0x8F,0x4F,0xCF,0x2F,0xAF,0x6F,0xEF,
        0x1F,0x9F,0x5F,0xDF,0x3F,0xBF,0x7F,0xFF
};

int write_HFE_file(HXCFLOPPYEMULATOR* floppycontext,FLOPPY * floppy,char * filename,int forceifmode)
{	

	pictrack * track;

	FILE * hxcpicfile;

	picfileformatheader *FILEHEADER,*TEMPHEADER;
	unsigned char *mfmtracks0,*mfmtracks1,*mfmtrackfinal;
	unsigned char *offsettrack, *tempoffsettrack;
	int mfmsize,mfmsize2;
	unsigned int i,j,k;
	unsigned int trackpos;
	unsigned int tracklistlen;
	unsigned int tracksize;

	floppycontext->hxc_printf(MSG_INFO_1,"Write HFE file %s for the standalone emulator.",filename);

	hxcpicfile=fopen(filename,"wb");

	if(hxcpicfile)
	{
		FILEHEADER=(picfileformatheader *) malloc(512);
		memset(FILEHEADER,0xFF,512);
		sprintf((char*)&FILEHEADER->HEADERSIGNATURE,"HXCPICFE");

		FILEHEADER->number_of_track=floppy->floppyNumberOfTrack;
		FILEHEADER->number_of_side=floppy->floppyNumberOfSide;
		FILEHEADER->bitRate=floppy->floppyBitRate/1000;
		FILEHEADER->floppyRPM=0;
		if(forceifmode==-1)
		{
			FILEHEADER->floppyinterfacemode=floppy->floppyiftype;
		}
		else
		{
			FILEHEADER->floppyinterfacemode=forceifmode;
		}
		FILEHEADER->track_encoding=0;
		FILEHEADER->formatrevision=0;
		FILEHEADER->track_list_offset=1;
		FILEHEADER->write_protected=1;
        
#ifdef BIGENDIAN
		/* Amgia is big endian, unlike x86
		   so we need to swap bytes here and there */
		   
		TEMPHEADER = (picfileformatheader*) malloc(512); 
		memcpy(TEMPHEADER,FILEHEADER,512);
		printf("%x, %x\n", FILEHEADER->bitRate, simpleswab16(FILEHEADER->bitRate)); 

		TEMPHEADER->bitRate=simpleswab16(FILEHEADER->bitRate);
		TEMPHEADER->track_list_offset=simpleswab16(FILEHEADER->track_list_offset);

		fwrite(TEMPHEADER,512,1,hxcpicfile);
		free(TEMPHEADER);
#else
		fwrite(FILEHEADER,512,1,hxcpicfile);
#endif
		tracklistlen=(((((FILEHEADER->number_of_track+1)*sizeof(pictrack))/512)+1));
		offsettrack=(unsigned char*) malloc(tracklistlen*512);
		memset(offsettrack,0xFF,tracklistlen*512);
		
		i=0;
		trackpos=FILEHEADER->track_list_offset+tracklistlen;

		do
		{
				mfmsize=0;
				mfmsize2=0;


				mfmsize=floppy->tracks[i]->sides[0]->tracklen;
				if(floppy->tracks[i]->number_of_side==2)
				{
					mfmsize2=floppy->tracks[i]->sides[1]->tracklen;
				}

				if(mfmsize2>mfmsize) mfmsize=mfmsize2;
			
				track=(pictrack *)(offsettrack+(i*sizeof(pictrack)));
				if(mfmsize*2>0xFFFF)
				{
					floppycontext->hxc_printf(MSG_ERROR,"Argg!! track %d too long (%x) and shorten to 0xFFFF !",i,mfmsize*2);
					mfmsize=0x7FFF;
				}
				track->track_len=mfmsize*2;
				track->offset=trackpos;


				if((mfmsize*2)%512)
					trackpos=trackpos+(((mfmsize*2)/512)+1);
				else
					trackpos=trackpos+((mfmsize*2)/512);

				
			i++;
		}while(i<(FILEHEADER->number_of_track));

#ifdef BIGENDIAN
		/* Amgia is big endian, unlike x86
		   so we need to swap bytes here and there */
		tempoffsettrack=(unsigned char*) malloc(tracklistlen*512);
		memset(tempoffsettrack,0xFF,tracklistlen*512);
		memcpy(tempoffsettrack,offsettrack,tracklistlen*512);
		swab(offsettrack,tempoffsettrack,tracklistlen*512); 
		fwrite(tempoffsettrack,512,1,hxcpicfile);
#else
		fwrite(offsettrack,512*tracklistlen,1,hxcpicfile);
#endif

		i=0;
		do
		{

				mfmsize=floppy->tracks[i]->sides[0]->tracklen;
				mfmsize2=0;
				if(floppy->tracks[i]->number_of_side==2)
				{
					mfmsize2=floppy->tracks[i]->sides[1]->tracklen;
				}
				if(mfmsize>0x7FFF)
				{
					mfmsize=0x7FFF;
				}
				if(mfmsize2>0x7FFF)
				{
					mfmsize2=0x7FFF;
				}
				track=(pictrack *)(offsettrack+(i*sizeof(pictrack)));

				if(track->track_len%512)
					tracksize=((track->track_len&(~0x1FF))+0x200)/2;
				else
					tracksize=track->track_len/2;

				mfmtracks0=(unsigned char*) malloc(tracksize);
				mfmtracks1=(unsigned char*) malloc(tracksize);
				mfmtrackfinal=(unsigned char*) malloc(tracksize*2);

				memset(mfmtracks0,0x00,tracksize);
				memset(mfmtracks1,0x00,tracksize);
				memset(mfmtrackfinal,0x55,tracksize*2);

				memcpy(mfmtracks0,floppy->tracks[i]->sides[0]->databuffer,mfmsize);
				
				if(floppy->tracks[i]->number_of_side==2)
					memcpy(mfmtracks1,floppy->tracks[i]->sides[1]->databuffer,mfmsize2);
				
				for(k=0;k<tracksize/256;k++)
				{

					for(j=0;j<256;j++)
					{
						mfmtrackfinal[(k*512)+j]=     bit_inverter[mfmtracks0[(k*256)+j]];
						mfmtrackfinal[(k*512)+j+256]= bit_inverter[mfmtracks1[(k*256)+j]];

					}
				}
				
				
				fwrite(mfmtrackfinal,tracksize*2,1,hxcpicfile);
					
				free(mfmtracks0);
				free(mfmtracks1);
				free(mfmtrackfinal);

		
			i++;
		}while(i<(FILEHEADER->number_of_track));

		free(offsettrack);

        fclose(hxcpicfile);
		
		floppycontext->hxc_printf(MSG_INFO_1,"%d tracks written to the file",FILEHEADER->number_of_track);

		free(FILEHEADER);

		return 0;
	}
	else
	{
		floppycontext->hxc_printf(MSG_ERROR,"Cannot create %s!",filename);

		return -1;
	}

}
