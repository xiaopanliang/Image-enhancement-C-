/*===
lab4-skeleton.cc

Abstract: Skeleton for Lab 4
===*/

#include <stdlib.h>
#include <iostream>
#include <vector>
#include "image.h"
using namespace std;

int main (int argc, char* argv[])
{
	//declaring
	Image machImage;
	machImage.Resize (512, 512);
	Image hmachImage;
	hmachImage.Resize (512, 512);
	Image singrateImage;
	singrateImage.Resize (1024, 512);
	Image boxesImage;
	boxesImage.Resize (1024, 512);
	Image mboxesImage;
	mboxesImage.Resize (512, 512);
	
	
	//Image A1: Vertical Mach Bands
	for(int i=0; i<8; i++)
	{
		for(int w=0; w<64; w++)
		{
			for(int j=0; j<512;j++)
			{
			machImage.Pixel((i*64+w),j) = 255*i/7;
			}
		}
	}
	//Image A2: Horizental Mach Bands
	for(int i=0; i<512; i++)
	{
		for(int j=0; j<512; j++)
		{
		hmachImage.Pixel(i,j) = machImage.Pixel(j,i);
		}
	}
	
	//Image B: Sinusoidal Grating
	for(int i=0; i<1024; i++)
	{
		for(int j=0;j<512;j++)
		{
		singrateImage.Pixel(i,(511-j))=127.5*j/512*0.5*(1+cos((3.1415/3*(i+1)/1024)*i));
		}
	}
	
	//Image C: Simultaneous Contrast (S1=0,S2=255,X=128)
	for(int j=0; j<512; j++)
	{
		for(int i=0;i<1024;i++)
		{
			if(i<224)
			{
			boxesImage.Pixel(i,j)=0;
			}
			else if(i>287 && i<512)
			{
			boxesImage.Pixel(i,j)=0;
			}
			else if(i>511 && i<736)
			{
			boxesImage.Pixel(i,j)=255;
			}
			else if(i>799)
			{
			boxesImage.Pixel(i,j)=255;
			}
			else
			{
				if(j>223 &&j<288)
				{
				boxesImage.Pixel(i,j)=128;
				}
				else
				{
					if(i>223 && i<288)
					{
					boxesImage.Pixel(i,j)=0;
					}
					else
					{
					boxesImage.Pixel(i,j)=255;
					}
				}
			}
		}
	}
	
	//Image D: Weber's Law (F=255,I=0)
	for(int j=0; j<512; j++)
	{
		for(int i=0;i<512; i++)
		{
			if(i<252)
			{
			//I
			mboxesImage.Pixel(i,j)=0;
			}
			else if(i>259)
			{
			//I
			mboxesImage.Pixel(i,j)=0;
			}
			else
			{
				if(j>251 && j<260)
				{
				//F
				mboxesImage.Pixel(i,j)=255;
				}
				else
				{
				//I
				mboxesImage.Pixel(i,j)=0;
				}
			}
		}
	}
	
	//Plot Image
	machImage.SavePng ("ImageA1.png");
	hmachImage.SavePng ("ImageA2.png");
	singrateImage.SavePng ("ImageB.png");
	boxesImage.SavePng ("ImageC.png");
	mboxesImage.SavePng ("ImageD.png");
	return 0;
}
