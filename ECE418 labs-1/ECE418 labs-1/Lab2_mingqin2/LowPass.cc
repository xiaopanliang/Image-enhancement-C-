/*===
lab2c.cc
Lab 2 part (c) Low Pass Filtering
===*/

#include <stdlib.h>
#include <iostream>
#include <vector>
#include "image.h"
#include "fft.h"
using namespace std;

int main (int argc, char* argv[])
{
	// check parameters' correctness
	if (argc != 4) 
	{
		cerr << "Usage: " << argv[0] << " in.png out.png frequency" << endl;
		return 1;
	}
	double frequency = atof (argv[3]);
	if (frequency < 0 || frequency > 0.5)
	{
		cerr << "Cutoff frequency must be in [0,0.5]" << endl;
		return 1;
	}

	// load the image
	ComplexImage image;
	image.LoadPng (argv[1]);
	
	// Step1 Compute the 2D DFT of an image
	double m = 255.0;
	double M = -255.0;
	int col = image.Width();
	int row = image.Height();
	
	// Creat temporary vector,complex. and resize it to the size of the image's width
	vector<Complex> signal;
	signal.resize (col);
	
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
	
	
	// Step2 Perform the frequency domain operations
	// set all frequency samples outside the pass band to zero and leave all the samples inside the passband unchanged
	for (int i = 0; i < frequency*row; i++) 
	{
		for (int j = frequency*col; j < (1.00-frequency)*col; j++) 
		{
			image.Pixel(i, j) = 0;
		}
	}
	for (int i = frequency*row; i < (1.00-frequency)*row; i++) 
	{
		for (int j = 0; j < col; j++) 
		{
			image.Pixel(i,j) = 0;
		}
	}
	for (int i =(1.00-frequency)*row; i < row; i++) 
	{
		for (int j = frequency*col; j < (1.00-frequency)*col; j++) 
		{
			image.Pixel(i, j) = 0;
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
		for ( int j = 0; j < col; j++) 
		{
		image.Pixel(i,j) = (image.Pixel(i,j)-m)*255.0/(M-m);
		}
	}
	
	
	// save the image
	image.SavePng (argv[2]);
	
	return 0;
}
