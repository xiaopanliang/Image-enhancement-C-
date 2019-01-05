/*===
lab3a.cc

Lab 3 parts (a)
===*/

#include <stdlib.h>
#include <iostream>
#include <vector>
#include "imagefft.h"
using namespace std;

int main (int argc, char* argv[])
{
	// check parameters' correctness [for parts (a) and (b) -- this needs to be changed for (c)]
	if (argc < 3) 
	{
		cerr << "Usage: " << argv[0] << " in.png out.png [top] [left]" << endl;
		return 1;
	}

	// read the top row and left column that will be used [remove this for part (c)]
	int topLeftRow = atoi(argv[3]);
	int topLeftColumn = atoi(argv[4]);
	complex<double> pixel;
	//Step1: load the input image
	ComplexFFTImage inputImage;
	inputImage.LoadPng (argv[1]);
	
	//Step2: initialize the output interpolated image
	ComplexFFTImage outputImage;
	outputImage.Resize (512, 512);
	
	//Step3: replicate each pixel value in the subblock to filll its corresponding 4x4 subblock in the output image
	for(int i=0;i<128;i++)
	{
		for (int j=0;j<128;j++)
		{
		pixel = inputImage.Pixel(i+topLeftRow,j+topLeftColumn);
			for (int r=0;r<4;r++)
			{
				for (int c=0;c<4;c++)
				{
				outputImage.Pixel(i*4+r,j*4+c)=pixel;
				}
			}
		}
	}
	
	//Step4: save the image
	outputImage.SavePng (argv[2]);

	return 0;
}
