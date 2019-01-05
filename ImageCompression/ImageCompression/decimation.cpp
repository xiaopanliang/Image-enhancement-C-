#include <stdlib.h>
#include <iostream>
#include <vector>
#include "pch.h"
#include "imagefft.h"
using namespace std;

void decimation(const char *input, const char *restored, const char *difference, const char *output, const int factor, const int flag)
{
	//Declaring
	ComplexFFTImage inputImage;
	ComplexFFTImage inputoutImage;
	ComplexFFTImage restoredImage;
	ComplexFFTImage differenceImage;
	ComplexFFTImage outputImage;

	//Step1: load the input image
	inputImage.LoadPng(input);
	inputoutImage.LoadPng(input);

	//Declaring
	complex<int> pixela;
	complex<int> pixelb;

	double m = 255.0;
	double M = -255.0;

	vector<Complex> signal;
	int w = inputImage.Width() / factor;
	int h = inputImage.Height() / factor;

	complex<double> MSE = 0;
	complex<double> bbb = factor * factor;
	complex<double> p;
	outputImage.Resize(w, h);
	restoredImage.Resize(w*factor, h*factor);
	differenceImage.Resize(w*factor, h*factor);


	//Step2a: Decimation of input image with an Ideal Low Pass Filter
	if (flag > 1)
	{
		inputImage.FourierTransform();
		for (int i = 0; i < w / 2; i++)
		{
			for (int j = 0; j < h / 2; j++)
			{
				outputImage.Pixel(i, j) = inputImage.Pixel(i, j);
			}
		}
		for (int i = w / 2; i < w; i++)
		{
			for (int j = 0; j < h; j++)
			{
				outputImage.Pixel(i, j) = inputImage.Pixel(i + factor * w - w, j);
			}
		}
		for (int i = 0; i < w; i++)
		{
			for (int j = h / 2; j < h; j++)
			{
				outputImage.Pixel(i, j) = inputImage.Pixel(i, j + factor * h - h);
			}
		}
		for (int i = w / 2; i < w; i++)
		{
			for (int j = h / 2; j < h; j++)
			{
				outputImage.Pixel(i, j) = inputImage.Pixel(i + factor * w - w, j + factor * h - h);
			}
		}
		outputImage.InverseFourierTransform();

		for (int i = 0; i < w; i++)
		{
			for (int j = 0; j < h; j++)
			{
				outputImage.Pixel(i, j) = outputImage.Pixel(i, j).real() / bbb;

				if (outputImage.Pixel(i, j).real() > 255.0)
				{
					outputImage.Pixel(i, j).real()==255;
				}
				else if (outputImage.Pixel(i, j).real() <= 0)
				{
					outputImage.Pixel(i, j).real()==0;
				}
			}
		}
	}

	//Step2b: Decimation of input image without an Anti-aliasing Filter
	else if (flag <= 1)
	{
		for (int i = 0; i < w; i++)
		{
			for (int j = 0; j < h; j++)
			{
				outputImage.Pixel(i, j) = inputImage.Pixel(i*factor, j*factor);
			}
		}
	}

	//Step3: Use bilinear interpolationmethods to interpolate the small output image back to a size of 512 x 512
	//Row Interpolation
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			pixela = outputImage.Pixel(i, j);
			if (i != w - 1)
			{
				pixelb = outputImage.Pixel(i + 1, j);
			}
			else {
				pixelb = inputoutImage.Pixel(511, j);
			}
			signal.resize(factor + 1);
			for (int x = 0; x < (factor); x++)
			{
				signal[x] = (factor - x) * pixela / factor + x * pixelb / factor;
			}
			for (int c = 0; c < factor; c++)
			{
				restoredImage.Pixel((i*factor + c), (j*factor)) = signal[c];
			}
		}
	}
	//Column Interpolation
	for (int i = 0; i < w*factor; i++)
	{
		for (int j = 0; j < h; j++)
		{
			pixela = restoredImage.Pixel(i, j*factor);
			if (j != h - 1)
			{
				pixelb = restoredImage.Pixel(i, j*factor + factor);
			}
			else {
				pixelb = inputoutImage.Pixel(511, j);
			}
			signal.resize(factor + 1);
			for (int x = 0; x < (factor); x++)
			{
				signal[x] = (factor - x)*pixela / factor + x * pixelb / factor;
			}
			for (int r = 0; r < factor; r++)
			{
				restoredImage.Pixel(i, j*factor + r) = signal[r];
				//cout<< "MSE=" << endl<< " "<< restoredImage.Pixel(i,j)<<endl<<endl;
			}
		}
	}

	//Generate the difference image between the restored image and the input image
	for (int i = 0; i < w*factor; i++)
	{
		for (int j = 0; j < h*factor; j++)
		{
			differenceImage.Pixel(i, j) = inputoutImage.Pixel(i, j) - restoredImage.Pixel(i, j);
			//cout<< "diference" << endl<< " "<< differenceImage.Pixel(i,j)<<endl<<endl;
		}
	}

	//Calculate the MSE 
	for (int i = 0; i < 512; i++)
	{
		for (int j = 0; j < 512; j++)
		{
			MSE = MSE + pow(differenceImage.Pixel(i, j), 2);
		}
	}
	MSE = MSE / 512.0 / 512.0;
	cout << "MSE" << endl << " " << MSE << endl << endl;

	//Scaling the difference images
	for (int i = 0; i < w*factor; i++)
	{
		for (int j = 0; j < h*factor; j++)
		{
			m = min(m, differenceImage.Pixel(i, j).real());
			M = max(M, differenceImage.Pixel(i, j).real());
			//cout<< "m=" << endl<< " "<< m<<endl<<endl;
			//cout<< "M=" << endl<< " "<< M<<endl<<endl;

		}
	}

	for (int i = 0; i < w*factor; i++)
	{
		for (int j = 0; j < h*factor; j++)
		{
			if (differenceImage.Pixel(i, j).real() <= 0.0)
			{
				differenceImage.Pixel(i, j) = (differenceImage.Pixel(i, j) - m)*128.0 / (-m);
			}
			else if (differenceImage.Pixel(i, j).real() > 0.0)
			{
				differenceImage.Pixel(i, j) = (differenceImage.Pixel(i, j))*127.0 / (M)+128.0;
			}
			//cout<< "a=" << endl<< " "<< differenceImage.Pixel(i,j)<<endl<<endl;
		}
	}

	//Step4: save the image
	outputImage.SavePng(restored);
	restoredImage.SavePng(difference);
	differenceImage.SavePng(output);
}