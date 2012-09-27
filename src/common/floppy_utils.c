#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#include "hxc_floppy_emulator.h"
#include "internal_floppy.h"
#include "floppy_loader.h"

unsigned long us2index(unsigned long startindex,SIDE * track,unsigned long us,char fill,char fillorder)
{
	unsigned long time,freq;

	if(!fillorder)
	{
		if(track->bitrate==VARIABLEBITRATE)
		{
			time=0;
			do
			{

				if(fill)track->indexbuffer[startindex]=0xFF;
				freq=track->timingbuffer[startindex];
				startindex++;
				if(startindex>=track->tracklen) startindex=0;

				time=time+((1000000000/freq)*4);
			}while(us>(time/1000));
			return startindex;
		}
		else
		{
			freq=track->bitrate;
			time=0;
			do
			{
				if(fill)track->indexbuffer[startindex]=0xFF;
				startindex++;
				if(startindex>=track->tracklen) startindex=0;
				time=time+((1000000000/freq)*4);
			}while(us>(time/1000));
			return startindex;
		}
	}
	else
	{
		if(track->bitrate==VARIABLEBITRATE)
		{
			time=0;
			do
			{

				if(fill)track->indexbuffer[startindex]=0xFF;
				freq=track->timingbuffer[startindex];

				if(startindex)
					startindex--;
				else
					startindex=track->tracklen-1;
				
				time=time+((1000000000/freq)*4);
			}while(us>(time/1000));
			return startindex;
		}
		else
		{
			freq=track->bitrate;
			time=0;
			do
			{
				if(fill)track->indexbuffer[startindex]=0xFF;

				if(startindex)
					startindex--;
				else
					startindex=track->tracklen-1;

				time=time+((1000000000/freq)*4);
			}while(us>(time/1000));
			return startindex;
		}




	}

		
}

unsigned long fillindex(unsigned long startindex,SIDE * track,unsigned long us,char fillorder)
{
	
	return us2index(startindex,track,us,1,fillorder);
}

