#include <stdlib.h>
#include <iostream>
#include <vector>
#include "pch.h"
#include "image.h"
using namespace std;

void ImageHistogram(const char *input, const char *output, const float binCount)
{

	// load the input image
	Image image;
	image.LoadPng(input);

	int intensity;

	// set up the histogram
	vector<int> histogram(binCount, 0);

	//Initialize the histogram for any binCount
	for (int i = 0; i < binCount; i++)
	{
		histogram[i] = 0;
	}

	// Calculate the image's histogram
	for (int x = 0; x < image.Width(); x++)
	{
		for (int y = 0; y < image.Height(); y++)
		{

			intensity = image.Pixel(x, y);
			histogram[(intensity / (256 / binCount))] = histogram[(intensity / (256 / binCount))] + 1;
		}
	}

	// print the histogram the specified file
	ofstream out(output);
	if (out.fail())
	{
		cerr << "Failed to open file for histogram output" << endl;
		return;
	}
	for (int i = 0; i < histogram.size(); i++)
	{
		out << histogram[i] << endl;
	}
	out.close();
}