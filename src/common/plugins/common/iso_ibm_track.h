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
#define DEFAULT_HD_BITRATE 500000
#define DEFAULT_DD_BITRATE 250000
#define DEFAULT_DD_RPM 300

typedef struct SECTORCONFIG_
{
	unsigned int   head;
	unsigned int   sector;
	unsigned int   cylinder;
	unsigned int   sectorsize;
	unsigned char  badheadercrc;
	unsigned short header_crc;
	unsigned char  baddatacrc;
	unsigned char  missingdataaddressmark;
	
	unsigned char  use_alternate_datamark;
	unsigned char  alternate_datamark;
	unsigned char  use_alternate_addressmark;
	unsigned char  alternate_addressmark;

	unsigned long startsectorindex;
	unsigned long startdataindex;
}SECTORCONFIG;

typedef struct isoibm_config_
{
	 	int indexformat;

	 	unsigned char	data_gap4a;
	unsigned int	len_gap4a;

	 	unsigned char	data_isync;
	unsigned int	len_isync;

	 	unsigned char	data_indexmarkp1;
	unsigned char	clock_indexmarkp1;
	unsigned char	len_indexmarkp1;

	unsigned char	data_indexmarkp2;
	unsigned char	clock_indexmarkp2;
	unsigned char	len_indexmarkp2;

	 
	unsigned char	data_gap1;
	unsigned int	len_gap1;

	 	unsigned char	data_ssync;
	unsigned int	len_ssync;

	 	unsigned char	data_dsync;
	unsigned int	len_dsync;

	 	unsigned char	data_addrmarkp1;
	unsigned char	clock_addrmarkp1;
	unsigned char	len_addrmarkp1;

	unsigned char	data_addrmarkp2;
	unsigned char	clock_addrmarkp2;
	unsigned char	len_addrmarkp2;

	 	unsigned char	data_gap2;
	unsigned int	len_gap2;

	 	unsigned char	data_datamarkp1;
	unsigned char	clock_datamarkp1;
	unsigned char	len_datamarkp1;

	unsigned char	data_datamarkp2;
	unsigned char	clock_datamarkp2;
	unsigned char	len_datamarkp2;

	 	unsigned char	data_gap3;
	unsigned int	len_gap3;

	unsigned char	data_gap4b;
	unsigned int	len_gap4b;
	
}isoibm_config;

#define IBMFORMAT_SD 0x1
#define IBMFORMAT_DD 0x2
#define ISOFORMAT_SD 0x3
#define ISOFORMAT_DD 0x4
#define ISOFORMAT_DD11S 0x5

static isoibm_config formatstab[]=
{     	{	
		IBMFORMAT_SD,	 
			
		0xFF,40,  		
		0x00, 6,  		
		0x00,0x00,0, 		0xFC,0xD7,1, 
		
		0xFF,26,  		
		0x00, 6,  		
		0x00, 6,  		
		0x00,0x00,0, 		0xFE,0xC7,1, 
		
		0xFF,11,  		
		0x00,0x00,0, 		0xFB,0xC7,1, 
		
		0xFF,84,  		
		0xFF,255  	},
	
	
	{	
		IBMFORMAT_DD,	 
		
		0x4E,80,  		
		0x00,12,  		
		0xC2,0x14,3, 		0xFC,0xFF,1, 
		
		0x4E,50,  		
		0x00,12,  		
		0x00,12,  		
		0xA1,0x0A,3, 		0xFE,0xFF,1, 
		
		0x4E,22,  		
		0xA1,0x0A,3, 		0xFB,0xFF,1, 
		
		0x4E,84,  		
		0x4E,255  		
	},
	
	{	
		ISOFORMAT_SD,
		
		0xFF,00,  		
		0x00,00,  		
		0x00,0x00,0, 		0xFC,0xD7,1,
		
		0xFF,16,  		
		0x00,06,  		
		0x00,06,  		
		0x00,0x00,0, 		0xFE,0xC7,1,
		
		0xFF,11,  		
		0x00,0x00,0, 		0xF8,0xC7,1,
		
		0xFF,84,  		
		0xFF,255  		
	},
	
	{	
		ISOFORMAT_DD,
		
		0x4E,00,  		
		0x00,00,  		
		0x00,0x00,0, 		0x00,0x00,0, 
		
		0x4E,32,  		
		0x00,12,  		
		0x00,12,  		
		0xA1,0x0A,3, 		0xFE,0xFF,1,
		
		0x4E,22,  		
		0xA1,0x0A,3, 		0xFB,0xFF,1,
		
		0x4E,84,  		0x4E,255  		
	},	
	{	
		ISOFORMAT_DD11S,
		
		0x4E,00,  		
		0x00,00,  		
		0x00,0x00,0, 		0x00,0x00,0, 
		
		0x4E,00,  		
		0x00,03,  		
		0x00,12,  		
		0xA1,0x0A,3, 		0xFE,0xFF,1,
		
		0x4E,22,  		
		0xA1,0x0A,3, 		0xFB,0xFF,1,
		
		0x4E,5,   		
		0x4E,0xFF  		
	},	
	{	
		0,
		
		0x4E,00,  		
		0x00,00,  		
		0x00,0x00,0, 		0x00,0x00,0,
		
		0x4E,32,  		
		0x00,12,
		
		0x00,12,
		
		0xA1,0xFF,3,
		0xFE,0xFF,1,
		
		0x4E,22,
		
		0xA1,0xFF,3,
		0xFB,0xFF,1,
		
		0x4E,84,
		
		0x4E,255
		}
};

int BuildCylinder(unsigned char * mfm_buffer,int mfm_size,unsigned char * track_clk,unsigned char * track_data,int track_size);
void BuildFMCylinder(char * buffer,int fmtracksize,char * bufferclk,char * track,int size);
int BuildISOTrack(HXCFLOPPYEMULATOR* floppycontext,int TRACKTYPE,unsigned int numberofsector,unsigned char startidsector,unsigned int sectorsize,unsigned int tracknumber,unsigned int sidenumber,unsigned int gap3len,char* datain,unsigned char * mfmdata,unsigned long * mfmsizebuffer,int interleave,SECTORCONFIG * sectorconfigtab);

int ISOIBMGetTrackSize(int TRACKTYPE,unsigned int numberofsector,unsigned int sectorsize,unsigned int gap3len,SECTORCONFIG * sectorconfigtab);

