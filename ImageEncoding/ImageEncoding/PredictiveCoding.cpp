#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include "pch.h"
#include "image.h"
#include "log2.h"
#include "SnakePixel.h"
using namespace std;

void PredictiveCoding(const char *input, const char *output)
{
	// load the input image
	Image image;
	image.LoadPng(input);
	int totalPixels = image.Width() * image.Height();

	// a. Save original pixel vlaues in vector oripixelvlaue
	vector<double> oripixelvalue(totalPixels, 0);
	int apixel = 1;
	oripixelvalue[0] = image.Pixel(0, 0);
	for (int x = 0, y = 0; SnakePixel(image, x, y); )
	{
		oripixelvalue[apixel] = image.Pixel(x, y);
		apixel = apixel + 1;
	}

	// b.Save differences between the origianl and predict pixel values in vector
		// diffpixelvlue
	vector<double> diffpixelvalue(totalPixels - 1, 0);
	for (int x = 0; x < totalPixels - 1; x++)
	{
		diffpixelvalue[x] = oripixelvalue[x + 1] - oripixelvalue[x] + 255;
	}

	// c.create the normalized (sum of elements is one) histogram for prediction errors
	vector<double> predictionErrorHistogram(511, 0);
	int histogramx;

	for (int x = 0; x < totalPixels - 1; x++)
	{
		histogramx = diffpixelvalue[x];
		predictionErrorHistogram[histogramx] = predictionErrorHistogram[histogramx] + 1.0;
	}

	// d.calculate the probabilities
	vector<double> probofi(511, 0.0);
	for (int x = 0; x < 511; x++)
	{
		probofi[x] = predictionErrorHistogram[x] / (512 * 512 - 1);
	}

	// e. calculate the entropy
	double entropy = 0;
	for (int i = 0; i < predictionErrorHistogram.size(); i++)
	{
		entropy = entropy - probofi[i] * log2(probofi[i]);
	}

	cout << "Prediction Error Entropy is " << entropy << "." << endl;

	// print additional needed data
	double totalbits = entropy * (512.0*512.0 - 1.0);
	cout << "Prediction Error Entropy is " << totalbits << "." << endl;

	// print the histogram to the specified file
	ofstream out(output);
	if (out.fail())
	{
		cerr << "Failed to open file for histogram output" << endl;
		return;
	}
	for (int i = 0, pixels = 0; i < predictionErrorHistogram.size(); i++)
	{
		out << predictionErrorHistogram[i] << endl;
	}
	out.close();
}
