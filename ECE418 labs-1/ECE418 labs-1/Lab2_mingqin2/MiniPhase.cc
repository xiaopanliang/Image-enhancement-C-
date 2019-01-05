/*===
lab2a.cc
Lab 2 parts (a) Vanishing Phase
===*/

#include "pch.h"
#include "image.h"
#include "fft.h"
#include <stdlib.h>
#include <iostream>
#include <vector>

using namespace std;

int main (int argc, char* argv[])
{
	// check parameters' correctness
	if (argc != 3) 
	{
		cerr << "Usage: " << argv[0] << " in.png out.png" << endl;
		return 1;
	}

	// load the image
	ComplexImage image;
	image.LoadPng (argv[1]);
	
	// Step1 Compute the 2D DFT of an image
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
	for (int i = 0; i < row; i++) 
	{
		for (int j = 0; j < col; j++) 
		{
			// Remove the phase of each omplex frequency sample, preserve the magnitude
			image.Pixel(i, j) = abs(image.Pixel(i,j));
		}
	}
	

	// Step3 Compute the inverse 2D FFT of the image
	// Row by row, copy the row's contents to the vector<complex>
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			signal[j] = image.Pixel(i, j);
		}
		//Perform Inverse DFT
		FFT::InverseTransform(signal);
		//copy the data back to the row it came from
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
		//Perform Inverse DFT
		FFT::InverseTransform(signal);
		//copy the data back to the column it came from
		for (int i = 0; i < row; i++) 
		{
			image.Pixel(i, j) = signal[i];
		}
	}
	
	// save the image
	image.SavePng (argv[2]);
	
	return 0;
}
