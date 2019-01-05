/*===
lab7-part1.cc

lab7-part1
===*/

#include <stdlib.h>
#include <iostream>
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
	ComplexImage image;
	image.LoadPng (argv[1]);
	
	// perform gamma point operation
	for (int x = 0; x < image.Width (); x++)
	{
		for (int y = 0; y < image.Height (); y++)
		{
			//Gamma correction point operation for each pixel
			image.Pixel(x,y)=pow((image.Pixel(x,y)/255.0),2.5)*255.0;
			//cout<< "Pixel=" << endl<< " "<< image.Pixel(x,y)<<endl<<endl;
		}
	}
	
	// save the image to PNG format
	image.SavePng (argv[2]);

	return 0;
}
