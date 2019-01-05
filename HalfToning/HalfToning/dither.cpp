#include <stdlib.h>
#include <iostream>
#include <vector>
#include "pch.h"
#include "image.h"
using namespace std;

void dither(const char *input, const char *output)
{
	double MSE = 0;
	double differ = 0;

	// load the input image
	Image image;
	image.LoadPng(input);

	Image imagec;
	imagec.LoadPng(input);

	// do 1x1 dithering
	for (int x = 0; x < image.Width(); x++)
	{
		for (int y = 0; y < image.Height(); y++)
		{
			int& value = image.Pixel(x, y);
			value = (value < 128) ? 0 : 255;
			image.Pixel(x, y) = value;
		}
	}
	//Calculate the MSE 
	for (int i = 0; i < 512; i++)
	{
		for (int j = 0; j < 512; j++)
		{
			differ = image.Pixel(i, j) - imagec.Pixel(i, j);
			MSE = MSE + pow(differ, 2);
		}
	}
	MSE = MSE / 512.0 / 512.0;
	cout << "MSE" << endl << " " << MSE << endl << endl;
	// save the image
	image.SavePng(output);
}