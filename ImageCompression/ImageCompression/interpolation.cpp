#include <stdlib.h>
#include <iostream>
#include <vector>
#include "pch.h"
#include "image.h"
#include "imagefft.h"
using namespace std;

void interpolation(const char *input, const char *output, float topLeftRow, float topLeftColumn)
{
	complex<double> pixel;
	//Step1: load the input image
	ComplexFFTImage inputImage;
	inputImage.LoadPng(input);

	//Step2: initialize the output interpolated image
	ComplexFFTImage outputImage;
	outputImage.Resize(512, 512);

	//Step3: replicate each pixel value in the subblock to filll its corresponding 4x4 subblock in the output image
	for (int i = 0; i < 128; i++)
	{
		for (int j = 0; j < 128; j++)
		{
			pixel = inputImage.Pixel(i + topLeftRow, j + topLeftColumn);
			for (int r = 0; r < 4; r++)
			{
				for (int c = 0; c < 4; c++)
				{
					outputImage.Pixel(i * 4 + r, j * 4 + c) = pixel;
				}
			}
		}
	}

	//Step4: save the image
	outputImage.SavePng(output);
}
