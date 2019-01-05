#include <stdlib.h>
#include <iostream>
#include <vector>
#include "image.h"
#include "pch.h"
#include "fft.h"
using namespace std;

void MiniMagnitudeVariation(const char *input, const char *output)

{
	// load the image
	ComplexImage image;
	image.LoadPng(input);

	// Step1 Compute the 2D DFT of an image
	double m = 255.0;
	double M = -255.0;
	int col = image.Width();
	int row = image.Height();

	// Creat temporary vector,complex. and resize it to the size of the image's width
	vector<Complex> signal;
	signal.resize(col);

	// Row by row, copy the row's contents to the vector<complex>
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			signal[j] = image.Pixel(i, j);
		}
		// Perform DFT
		FFT::Transform(signal);
		// copy the data back to the row it came from
		for (int j = 0; j < col; j++)
		{
			image.Pixel(i, j) = signal[j];
		}
	}

	// Column by column, copy the column's content to the vector<complex>
	for (int j = 0; j < col; j++)
	{
		for (int i = 0; i < row; i++)
		{
			signal[i] = image.Pixel(i, j);
		}
		// Perform FFT
		FFT::Transform(signal);
		// copy the data back to the column it came from
		for (int i = 0; i < row; i++)
		{
			image.Pixel(i, j) = signal[i];

		}
	}


	// Step2 Perform the frequency domain operation(s)
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			// set the magnitude ot each complx frequency in the Fourier domain to one, preserve the phase
			image.Pixel(i, j) = image.Pixel(i, j) / abs(image.Pixel(i, j));
		}
	}


	// Step3 Compute the inverse 2D FFT of the image
	// Row by row, copy the row's contents to the vector<complex>
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			signal[j] = image.Pixel(i, j);
		}
		// Perform Inverse DFT
		FFT::InverseTransform(signal);
		// copy the data back to the row it came from
		for (int j = 0; j < col; j++) {
			image.Pixel(i, j) = signal[j];

		}
	}

	// Column by Column, copy the row's contents to the vector<complex>
	for (int j = 0; j < col; j++) {
		for (int i = 0; i < row; i++)
		{
			signal[i] = image.Pixel(i, j);
		}
		// Perform Inverse DFT
		FFT::InverseTransform(signal);
		// copy the data back to the column it came from
		for (int i = 0; i < row; i++)
		{
			image.Pixel(i, j) = signal[i];
		}
	}

	// Step4 re-normalize the image
	// Find the minimum and maximum value m and M in the real part of the Inverse FFT
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			m = min(m, image.Pixel(i, j).real());
			M = max(M, image.Pixel(i, j).real());
		}
	}

	// Modify the pixel value
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			image.Pixel(i, j) = (image.Pixel(i, j) - m)*255.0 / (M - m);
		}
	}

	// save the image
	image.SavePng(output);
}