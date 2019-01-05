/*===
lab7-part3.cc

Abstract: lab7-part3
===*/

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "image.h"
using namespace std;

int main (int argc, char* argv[])
{
	// verify arguments' correctness
	if (argc != 3)
	{
		cerr << "Useage: " << argv[0] 
		     << " input.png output.png" << endl;
		return 1;
	}

	// load the input image
	Image image;
	image.LoadPng (argv[1]);

	// Declaring
	int intensity;
	float min;

	// set up the histogram
	vector<int> histogram (256, 0);

	// Initialize the histogtam 
	for(int i=0; i<256; i++)
	{
		histogram[i]=0;
	}

	// calculate the image's histogram
	for (int x = 0; x < image.Width (); x++)
	{
		for (int y = 0; y < image.Height (); y++)
		{
			intensity = image.Pixel(x,y);
			histogram[intensity] = histogram[intensity] + 1;
		}
	}

	// Find the minimum value of the histogram
	min = histogram[0];
	for(int i=0; i<256; i++)
	{
		if(min > histogram[i])
		{
			min = histogram[i];
		}	
	}
	
	// calculate the cumulative distribution function from the histogram
	vector<float> cDistFunction (256, 0);
	for(int i=0; i<256; i++)
	{
		if(i>0)
		{
			cDistFunction[i]=cDistFunction[i-1] + histogram[i];
		}
		else
		{
			cDistFunction[i]=histogram[i];
		}
	}

	// apply the cumulative distribution function to equalize the image's histogram
	//  The histogram of equalized images are calculated using part2 with bin=256
	for (int x = 0; x < image.Width (); x++)
	{
		for (int y = 0; y < image.Height (); y++)
		{
			intensity = image.Pixel(x,y);
			image.Pixel(x,y) = (cDistFunction[intensity]-min)/(512*512-min)*255;
			//cout<< "pixel=" << endl<< " "<< image.Pixel(x,y)<<endl<<endl;
		}
	}

	// save the image 
	image.SavePng (argv[2]);

	return 0;
}
