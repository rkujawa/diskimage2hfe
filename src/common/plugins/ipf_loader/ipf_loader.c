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
///////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------//
//-----------H----H--X----X-----CCCCC----22222----0000-----0000------11----------//
//----------H----H----X-X-----C--------------2---0----0---0----0--1--1-----------//
//---------HHHHHH-----X------C----------22222---0----0---0----0-----1------------//
//--------H----H----X--X----C----------2-------0----0---0----0-----1-------------//
//-------H----H---X-----X---CCCCC-----222222----0000-----0000----1111------------//
//-------------------------------------------------------------------------------//
//----------------------------------------------------- http://hxc2001.free.fr --//
///////////////////////////////////////////////////////////////////////////////////
// File : IPF_DiskFile.c
// Contains: IPF floppy image loader and plugins interfaces
//
// Written by:	DEL NERO Jean Francois
//
// Change History (most recent first):
///////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "hxc_floppy_emulator.h"
#include "internal_floppy.h"
#include "floppy_loader.h"
#include "floppy_utils.h"

#include "ipf_loader.h"
#include "./libs/capslib/capsimage.h"

#include "../common/os_api.h"

int IPF_libIsValidDiskFile(HXCFLOPPYEMULATOR* floppycontext,char * imgfile)
{
	int pathlen;
	char * filepath;
	floppycontext->hxc_printf(MSG_DEBUG,"IPF_libIsValidDiskFile %s",imgfile);
	if(imgfile)
	{
		pathlen=strlen(imgfile);
		if(pathlen!=0)
		{
			filepath=malloc(pathlen+1);
			if(filepath!=0)
			{
				sprintf(filepath,"%s",imgfile);
				strlower(filepath);

				if(strstr( filepath,".ipf" )!=NULL)
				{
					floppycontext->hxc_printf(MSG_DEBUG,"IPF file !");
					free(filepath);
					return LOADER_ISVALID;
				}
				else
				{
					floppycontext->hxc_printf(MSG_DEBUG,"non IPF file !");
					free(filepath);
					return LOADER_BADFILE;
				}
			}
		}
	}

	return LOADER_BADPARAMETER;
}



int IPF_libLoad_DiskFile(HXCFLOPPYEMULATOR* floppycontext,FLOPPY * floppydisk,char * imgfile,void * parameters)
{
	
	unsigned int filesize;
	unsigned int i,j,k,l,m;
	unsigned char *fileimg;
	typedef struct CapsImageInfo CapsImageInfo_;
	struct CapsTrackInfoT1 ti;
	struct CapsTrackInfoT1 flakeyti;
	unsigned char * temptrack;
	CapsImageInfo_ ci2;
	FILE * f;
	int img;
	int ret;
	int intrackflakeybit;
	unsigned long bitrate;
	int rpm;
	CYLINDER* currentcylinder;
	unsigned char flakeybyte;
	SIDE* currentside;
	
	CapsULong flag;
	
	floppycontext->hxc_printf(MSG_DEBUG,"IPF_libLoad_DiskFile %s",imgfile);
	
	f=fopen(imgfile,"rb");
	if(f==NULL) 
	{
		floppycontext->hxc_printf(MSG_ERROR,"Cannot open %s !",imgfile);
		return LOADER_ACCESSERROR;
	}
	
	fseek (f , 0 , SEEK_END); 
	filesize=ftell(f);
	fseek (f , 0 , SEEK_SET); 
	
	if(filesize!=0)
	{
		fileimg=(char*)malloc(filesize);
		
		if(fileimg!=NULL)
		{
			i=0;
			do
			{
				fread(fileimg+(i*1024),1024,1,f);
				i++;
			}while(i<((filesize/1024)+1));
			
		}
		else
		{
			floppycontext->hxc_printf(MSG_ERROR,"Memory error!");
			fclose(f);
			return LOADER_INTERNALERROR;
		}	
	}
	else
	{
		floppycontext->hxc_printf(MSG_ERROR,"0 byte file!");
		fclose(f);
		return LOADER_BADFILE;
	}
	
	fclose(f);
	
	flag=DI_LOCK_DENVAR|DI_LOCK_UPDATEFD|DI_LOCK_TYPE;
	img=CAPSAddImage();
	
	if(img!=-1)
	{
		if(CAPSLockImageMemory(img, fileimg,filesize,0)!=imgeUnsupported )
		{
			CAPSLoadImage(img, flag);
			CAPSGetImageInfo(&ci2, img);
			////// debug //////
			floppycontext->hxc_printf(MSG_DEBUG,"Image Info: %s",imgfile);
			floppycontext->hxc_printf(MSG_DEBUG,"Type     : %.8x",ci2.type);
			floppycontext->hxc_printf(MSG_DEBUG,"Release  : %.8x",ci2.release);
			floppycontext->hxc_printf(MSG_DEBUG,"Revision : %.8x",ci2.revision);
			floppycontext->hxc_printf(MSG_DEBUG,"Platform : %.8x",ci2.platform);
			floppycontext->hxc_printf(MSG_DEBUG,"minhead  : %d",ci2.minhead);
			floppycontext->hxc_printf(MSG_DEBUG,"maxhead  : %d",ci2.maxhead);
			floppycontext->hxc_printf(MSG_DEBUG,"mincylinder : %d",ci2.mincylinder);
			floppycontext->hxc_printf(MSG_DEBUG,"maxcylinder : %d",ci2.maxcylinder);
			floppycontext->hxc_printf(MSG_DEBUG,"Date : %d/%d/%d",ci2.crdt.day,ci2.crdt.month,ci2.crdt.year);
			///////////////////
			
			
			if(ci2.type == ciitFDD)
			{
				floppydisk->floppySectorPerTrack=0;
				floppydisk->floppyNumberOfSide=(ci2.maxhead-ci2.minhead)+1;
				floppydisk->floppyNumberOfTrack=ci2.maxcylinder+1;
				floppydisk->floppyBitRate=250000;
				rpm=300;
				floppydisk->floppyiftype=AMIGA_DD_FLOPPYMODE;
				floppydisk->tracks=(CYLINDER**)malloc(sizeof(CYLINDER*)*floppydisk->floppyNumberOfTrack);
				memset(floppydisk->tracks,0,sizeof(CYLINDER*)*floppydisk->floppyNumberOfTrack);
				
				for(i=ci2.mincylinder;i<=ci2.maxcylinder;i++)
				{
					for(j=ci2.minhead;j<=ci2.maxhead;j++)
					{
						ti.type = LIB_TYPE;
						floppycontext->hxc_printf(MSG_DEBUG,"-----------------------------%d %d",i,j);
						ret=CAPSLockTrack((struct CapsTrackInfo *)&ti, img, i, j, flag);
						if(ret==imgeOk)
						{
							floppycontext->hxc_printf(MSG_DEBUG,"Track Info  : %d %d",i,j);
							floppycontext->hxc_printf(MSG_DEBUG,"Cylinder    : %d",ti.cylinder);
							floppycontext->hxc_printf(MSG_DEBUG,"Head        : %d",ti.head);
							floppycontext->hxc_printf(MSG_DEBUG,"Sectorcnt   : %d",ti.sectorcnt);
							floppycontext->hxc_printf(MSG_DEBUG,"sectorsize  : %d",ti.sectorsize);
							floppycontext->hxc_printf(MSG_DEBUG,"Type        : %.8X",ti.type);
							//	floppycontext->hxc_printf(MSG_DEBUG,"trackcnt    : %d\n",ti.trackcnt);
							floppycontext->hxc_printf(MSG_DEBUG,"tracklen     : %d",ti.tracklen);
							floppycontext->hxc_printf(MSG_DEBUG,"overlap     : %d",ti.overlap);
														
							if(!floppydisk->tracks[i])
							{
								floppydisk->tracks[i]=(CYLINDER*)malloc(sizeof(CYLINDER));
								currentcylinder=floppydisk->tracks[i];
								currentcylinder->number_of_side=floppydisk->floppyNumberOfSide;
								currentcylinder->sides=(SIDE**)malloc(sizeof(SIDE*)*currentcylinder->number_of_side);
								memset(currentcylinder->sides,0,sizeof(SIDE*)*currentcylinder->number_of_side);
								
								currentcylinder->floppyRPM=rpm;	
							}
							
							currentcylinder->sides[j]=malloc(sizeof(SIDE));
							memset(currentcylinder->sides[j],0,sizeof(SIDE));
							currentside=currentcylinder->sides[j];

							currentside->flakybitsbuffer=0;
							
							intrackflakeybit=-1;

							////////////////////////////////////////////////////////////////
							// get track data & flakey bit(s)
							if(!( (ti.type & CTIT_MASK_TYPE) == ctitNoise) && ti.trackbuf)
							{

								currentside->tracklen=ti.tracklen;
								currentside->databuffer=malloc(currentside->tracklen);
								memset(currentside->databuffer,0,currentside->tracklen);
								
								//flakey bits in track ?
								if(ti.type & CTIT_FLAG_FLAKEY)
								{

									temptrack=(unsigned char *)malloc(currentside->tracklen);
									memset(temptrack,0,currentside->tracklen);

									memcpy(currentside->databuffer,&ti.trackbuf[ti.overlap],ti.tracklen-ti.overlap);
									if(ti.overlap) memcpy(&currentside->databuffer[ti.tracklen-ti.overlap],&ti.trackbuf[0],ti.tracklen-(ti.tracklen-ti.overlap));

									currentside->flakybitsbuffer=malloc(currentside->tracklen);
									memset(currentside->flakybitsbuffer,0x00,currentside->tracklen);	

									CAPSUnlockTrack(img,i, j);
									
									// try to read the track x time, and check for differences
									for(k=0;k<10;k++)
									{
										flakeyti.type = LIB_TYPE;
										ret=CAPSLockTrack((struct CapsTrackInfo *)&flakeyti, img, i, j, flag);
										if(ret==imgeOk)
										{
											memcpy(temptrack,&flakeyti.trackbuf[flakeyti.overlap],flakeyti.tracklen-flakeyti.overlap);
											if(flakeyti.overlap) memcpy(&temptrack[flakeyti.tracklen-flakeyti.overlap],&flakeyti.trackbuf[0],flakeyti.tracklen-(flakeyti.tracklen-flakeyti.overlap));

											for(l=0;l<ti.tracklen;l++)
											{
												flakeybyte= temptrack[l] ^ currentside->databuffer[l];
												for(m=0;m<8;m=m+2)
												{
													// a flaybit detected ?
													if((flakeybyte&(0xC0>>m)) || ((currentside->databuffer[l]&(0xC0>>m))==(0xC0>>m) ) )
													{
														currentside->flakybitsbuffer[l]=currentside->flakybitsbuffer[l] | (0xC0>>m);
														currentside->databuffer[l]=currentside->databuffer[l] & ~(0xC0>>m);
														intrackflakeybit=l;
													}
												}
											

											}
											CAPSUnlockTrack(img,i, j);
										}
									}

									ti.type = LIB_TYPE;
									ret=CAPSLockTrack((struct CapsTrackInfo *)&ti, img, i, j, flag);

									free(temptrack);

								}
								else
								{	
									memcpy(currentside->databuffer,&ti.trackbuf[ti.overlap],ti.tracklen-ti.overlap);
									if(ti.overlap) memcpy(&currentside->databuffer[ti.tracklen-ti.overlap],&ti.trackbuf[0],ti.tracklen-(ti.tracklen-ti.overlap));
								}
							}
							else
							{
								currentside->tracklen=12500;
								currentside->databuffer=malloc(currentside->tracklen);
								memset(currentside->databuffer,0,currentside->tracklen);
								currentside->flakybitsbuffer=malloc(currentside->tracklen);
								memset(currentside->flakybitsbuffer,0xFF,currentside->tracklen);
							}
							
							bitrate=(rpm/60)*currentside->tracklen*4;							
						
							floppycontext->hxc_printf(MSG_DEBUG,"Fixed bitrate    : %d",bitrate);
							currentside->timingbuffer=0;
							currentside->bitrate=bitrate;

							////////////////////////////////////////////////////////////////
							// get track timing
							if(ti.timebuf!=0)
							{
								floppycontext->hxc_printf(MSG_DEBUG,"Variable bit rate!!!");
								
								currentside->timingbuffer=malloc(ti.timelen*sizeof(unsigned long));
								memset(currentside->timingbuffer,0,ti.timelen*sizeof(unsigned long));
								k=0;
								do
								{							
									currentside->timingbuffer[k]=((1000-ti.timebuf[k])*(bitrate/1000))+bitrate;
									k++;
								}while(k<ti.timelen);
								currentside->bitrate=VARIABLEBITRATE;
							}
								
							floppycontext->hxc_printf(MSG_DEBUG,"timebuf     : %.8X",ti.timebuf);
							
							if(ti.type & CTIT_FLAG_FLAKEY) 
							{
								floppycontext->hxc_printf(MSG_DEBUG,"Track %d Side 0: CTIT_FLAG_FLAKEY",i);
							}

							
							if((ti.type & CTIT_MASK_TYPE) ==  ctitNoise) 
							{
								floppycontext->hxc_printf(MSG_DEBUG,"Track %d Side 0: cells are unformatted (random size)",i);
							}
							

							if(intrackflakeybit !=  -1) 
							{
								floppycontext->hxc_printf(MSG_DEBUG,"In track flakey bit found (last: %d)",intrackflakeybit);
							}

							CAPSUnlockTrack(img,i, j);
							
							if(floppydisk->floppySectorPerTrack<ti.sectorcnt )
								floppydisk->floppySectorPerTrack=ti.sectorcnt;
							
							currentside->number_of_sector=ti.sectorcnt;

							
							currentside->indexbuffer=malloc(currentside->tracklen);
							memset(currentside->indexbuffer,0,currentside->tracklen);
									
							fillindex(currentside->tracklen-8,currentside,2400,0);
							//fillindex(currentside->tracklen-8,currentside,3200,0);

						}
						else
						{
							// error -> random track
							currentside->timingbuffer=0;
							currentside->tracklen=12500;
							currentside->databuffer=malloc(currentside->tracklen);
							memset(currentside->databuffer,0,currentside->tracklen);
							
							currentside->flakybitsbuffer=malloc(currentside->tracklen);
							memset(currentside->flakybitsbuffer,0xFF,currentside->tracklen);
						}
						
					}
					
				}				
				
			}
			
			CAPSUnlockAllTracks(img);
			
		}
		
		CAPSUnlockImage(img);
		
		
		CAPSRemImage(img);
		
		free(fileimg);
		
		floppycontext->hxc_printf(MSG_INFO_1,"IPF Loader : tracks file successfully loaded and encoded!");
		return LOADER_NOERROR;
		
		
	}
	
	
	return LOADER_INTERNALERROR;
}

