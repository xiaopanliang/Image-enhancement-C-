#include <stdlib.h>
#include <iostream>
#include <vector>
#include "pch.h"
#include "image.h"
#include "imagefft.h"
using namespace std;

void Bilinear_inter(const char *input, const char *output, float topLeftRow, float topLeftColumn)
{
	complex<int> pixela;
	complex<int> pixelb;
	vector<Complex> signal;
	//Step1: load the input image
	ComplexFFTImage inputImage;
	inputImage.LoadPng(input);

	//Step2: initialize the output interpolated image
	ComplexFFTImage outputImage;
	outputImage.Resize(512, 512);

	//Step3a: Interpolate linearly along each row between the copied pixels
	for (int i = 0; i < 128; i++)
	{
		for (int j = 0; j < 128; j++)
		{
			pixela = inputImage.Pixel(i + topLeftRow, j + topLeftColumn);
			pixelb = inputImage.Pixel(i + topLeftRow + 1, j + topLeftColumn);
			signal.resize(5);
			signal[0] = (4 / 4) * pixela + (0 / 4) * pixelb;
			signal[1] = 3 * pixela / 4 + pixelb / 4;
			signal[2] = 2 * pixela / 4 + 2 * pixelb / 4;
			signal[3] = pixela / 4 + 3 * pixelb / 4;
			signal[4] = (0 / 4) * pixela + (4 / 4) * pixelb;
			for (int c = 0; c < 4; c++)
			{
				outputImage.Pixel((i * 4 + c), (j * 4)) = signal[c];
			}
		}
	}
	//Step3b: Interpolate linearly along each column between the copied pixels
	for (int i = 0; i < 512; i++)
	{
		for (int j = 0; j < 128; j++)
		{
			pixela = outputImage.Pixel(i, j * 4);
			if (j != 127)
			{
				pixelb = outputImage.Pixel(i, j * 4 + 4);
			}
			else {
				pixelb = 0;
			}
			signal.resize(5);
			signal[0] = (4 / 4) * pixela + (0 / 4) * pixelb;
			signal[1] = 3 * pixela / 4 + pixelb / 4;
			signal[2] = 2 * pixela / 4 + 2 * pixelb / 4;
			signal[3] = pixela / 4 + 3 * pixelb / 4;
			signal[4] = (0 / 4) * pixela + (4 / 4) * pixelb;
			for (int r = 0; r < 4; r++)
			{
				outputImage.Pixel(i, j * 4 + r) = signal[r];
			}
		}
	}

	// save the image
	outputImage.SavePng(output);
}