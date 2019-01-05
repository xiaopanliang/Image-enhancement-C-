/*===

Pixelate: Average every blcok of pixel of image

===*/

#include <stdlib.h>
#include <iostream>
#include <vector>
#include "image.h"
using namespace std;

int main (int argc, char* argv[])
{
	// check parameters' correctness
	if (argc != 3) 
	{
		cerr << "Usage: lab1 in.png out.png" << endl;
		return 1;
	}

	// load the image
	Image image;
	image.LoadPng (argv[1]);

	/*==

	Insert Lab 1 Code Here
	
	==*/
	for (int r = 0; r < image.Height(); r += 4)
	{
		for (int c = 0; c < image.Width(); c += 4)
		{
			int sum = 0;
			//Claclulate sum of pixel of a 4x4 block
			for (int i = r; i < r+4; i++)
			{
				for (int j = c; j< c+4; j++)
				{
					sum += image.Pixel(i,j);
				}
			}
			
			//Calculate average pixel of this block
			int avg = sum/16;
			
			//Change all pixels in the block to avg
			for (int l = r; l < r+4; l++)
			{
				for (int k = c; k< c+4; k++)
				{
					image.Pixel(l,k) = avg;
				}
			}			
		}
	}
	
	// save it
	image.SavePng (argv[2]);
	
	return 0;
}
