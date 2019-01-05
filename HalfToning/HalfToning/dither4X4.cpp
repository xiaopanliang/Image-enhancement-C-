#include <stdlib.h>
#include <iostream>
#include <vector>
#include "pch.h"
#include "image.h"
using namespace std;

void dither4X4(const char *input, const char *output)
{
	// load the input image
	Image image;
	image.LoadPng(input);
	Image imagec;
	imagec.LoadPng(input);

	// the dithering matrix (to access the pixel at column x, row y, the
	// index in this structure is [x][y]
	static short ditherMatrix[4][4] = {
		{8,   200, 56,  248},
		{136, 72,  184, 120},
		{40,  232, 24,  216},
		{168, 104, 152, 88} };

	// Initialize a temporary 4x4 matrix
	static short signal1[4][4];;

	//Declaring
	int value;
	int thershold;
	double MSE = 0;
	double differ = 0;

	//Select 4x4 subblock in original image
	for (int i = 0; i < 512; i += 4)
	{
		for (int j = 0; j < 512; j += 4)
		{
			//Put pixel values in this subblock in 4x4 matrix
			for (int c = 0; c < 4; c++)
			{
				for (int r = 0; r < 4; r++)
				{
					signal1[c][r] = image.Pixel((i + c), (j + r));
				}
			}
			//Compare pixel values with dither matrix and quantize pixel values
			for (int m = 0; m < 4; m++)
			{
				for (int n = 0; n < 4; n++)
				{
					value = signal1[m][n];
					thershold = ditherMatrix[m][n];
					value = (value < thershold) ? 0 : 255;
					signal1[m][n] = value;
				}
			}

			//Put quantized pixel values in subblock back to image
			for (int l = 0; l < 4; l++)
			{
				for (int k = 0; k < 4; k++)
				{
					image.Pixel((i + l), (j + k)) = signal1[l][k];
				}
			}
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
