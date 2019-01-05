/*===
lab5-dither1x1.cc

Abstract: Implementation of a 1x1 dithering scheme
===*/

#include <stdlib.h>
#include <iostream>
#include <vector>
#include "image.h"
using namespace std;

int main (int argc, char* argv[])
{
	// load the input image
	Image image;
	image.LoadPng ("ca_coast.png");

	Image imagec;
	imagec.LoadPng ("ca_coast.png");

	double MSE;
	double differ;
	
	// do 1x1 dithering
	for (int x = 0; x < image.Width (); x++)
	{
		for (int y = 0; y < image.Height (); y++)
		{
			int& value = image.Pixel (x, y);
			value = (value < 128) ? 0 : 255;
			image.Pixel(x,y) = value;
		}
	}
	//Calculate the MSE 
	for (int i = 0; i < 512; i++) 
	{
		for ( int j = 0; j < 512; j++) 
		{
		differ = image.Pixel(i,j)-imagec.Pixel(i,j);
		MSE=MSE+pow(differ,2);
		}
	}
	MSE = MSE/512.0/512.0;
	cout<< "MSE" << endl<< " "<< MSE<<endl<<endl;
	// save the image
	image.SavePng ("ca_coast_dither1x1.png");

	return 0;
}
