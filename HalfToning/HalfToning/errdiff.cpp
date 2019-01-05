#include <stdlib.h>
#include <iostream>
#include <vector>
#include "pch.h"
#include "image.h"
using namespace std;

void errdiff(const char *input, const char *output)
{
	// load the input image
	ImageTemplate<double> image;
	image.LoadPng(input);
	ImageTemplate<double> imagec;
	imagec.LoadPng(input);

	//declaring Initialize error image and output image
	ImageTemplate<double> error;
	error.Resize(513, 512);

	ImageTemplate<double> outputimage;
	outputimage.Resize(512, 512);

	//Declaring
	double value;
	double recenterror;
	double output_value;
	double quanoutput;
	double MSE = 0;
	double differ = 0;

	//Step1: initialize the error image
	for (int i = 0; i < 513; i++)
	{
		for (int j = 0; j < 512; j++)
		{
			error.Pixel(i, j) = 0;
		}
	}

	//Step2: From top to bottom, left to right, accumulate error pixel at current pixel location x
	for (int j = 0; j < 512; j++)
	{
		for (int i = 0; i < 512; i++)
		{
			//pixel values at top and left edges is quantized qccording to 1x1 subblock quantization scheme
			if ((i == 0) || (j == 0))
			{
				value = image.Pixel(i, j);
				value = (value < 128) ? 0 : 255;
				outputimage.Pixel(i, j) = value;
			}
			else
			{
				//Calculate recent error at X by sunning all the terms around it in the error image
				recenterror = error.Pixel((i - 1), (j - 1)) / 16 + 5 * error.Pixel(i, (j - 1)) / 16 + 3 * error.Pixel((i + 1), (j - 1)) / 16 + 7 * error.Pixel((i - 1), j) / 16;
				error.Pixel(i, j) = recenterror;
				//Step3: Calculate output image value O, O = I-E
				output_value = image.Pixel(i, j) - error.Pixel(i, j);
				//Quantize the value O according to the 1x1 subblock quantization scheme
				quanoutput = (output_value < 128) ? 0 : 255;
				//Step4: Update the error image at location X with E = QO - O
				error.Pixel(i, j) = quanoutput - output_value;
				outputimage.Pixel(i, j) = quanoutput;
			}
		}
	}

	//Calculate the MSE 
	for (int i = 0; i < 512; i++)
	{
		for (int j = 0; j < 512; j++)
		{
			differ = outputimage.Pixel(i, j) - imagec.Pixel(i, j);
			MSE = MSE + pow(differ, 2);
		}
	}
	MSE = MSE / 512.0 / 512.0;
	cout << "MSE" << endl << " " << MSE << endl << endl;

	// save the image
	outputimage.SavePng(output);

}