/*===
lab10-part4-skeleton.cc

Abstract: Skeleton for part for of lab ten (predictive coding)
===*/

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "image.h"
using namespace std;

// calculate the log base 2 of a number using the convention log2(0)=0
double log2 (double x);

// snake raster scan function - updates x and y for a raster-snake scan
// of image, returning false only when the whole image has been traversed
bool SnakePixel (const Image& image, int& x, int& y);

// main routine
int main (int argc, char* argv[]) 
{
	// verify arguments' correctness
	if (argc != 3) 
	{
		cerr << "Useage: " << argv[0] 
		     << " input.png histogram.txt" << endl;
		return 1;
	}

	// load the input image
	Image image;
	image.LoadPng (argv[1]);
	int totalPixels = image.Width () * image.Height ();
	
	// a. Save original pixel vlaues in vector oripixelvlaue
	vector<double> oripixelvalue(totalPixels,0);
	int apixel = 1;
	oripixelvalue[0] = image.Pixel(0,0);
	for (int x = 0, y = 0; SnakePixel (image, x, y); )
	{
		oripixelvalue[apixel] = image.Pixel(x,y);
		apixel = apixel + 1;
	}

	// b.Save differences between the origianl and predict pixel values in vector
        // diffpixelvlue
	vector<double> diffpixelvalue(totalPixels-1,0);
	for (int x =0; x<totalPixels-1; x++)
	{
		diffpixelvalue[x] = oripixelvalue[x+1]-oripixelvalue[x]+255;
	}

	// c.create the normalized (sum of elements is one) histogram for prediction errors
	vector<double> predictionErrorHistogram (511, 0);
	int histogramx;
	
	for (int x = 0; x<totalPixels-1; x++)
	{
		histogramx = diffpixelvalue[x];
		predictionErrorHistogram[histogramx] = predictionErrorHistogram[histogramx]+1.0;
	}

	// d.calculate the probabilities
	vector<double> probofi (511,0.0);
	for (int x = 0; x < 511 ; x++)
	{
		probofi[x] = predictionErrorHistogram[x]/(512*512);
	}

	// e. calculate the entropy
	double entropy = 0;
	for (int i = 0; i < predictionErrorHistogram.size (); i++) 
	{
		entropy = entropy-probofi[i]*log2(probofi[i]);
	}	
	
	cout << "Prediction Error Entropy is " << entropy << "." << endl;
	
	// print additional needed data
	double totalbits = entropy*(512.0*512.0-1.0);
	cout << "Prediction Error Entropy is " << totalbits << "." << endl;

	// print the histogram to the specified file
	ofstream out (argv[2]);
	if (out.fail () ) 
	{
		cerr << "Failed to open file for histogram output" << endl;
		return 1;
	}
	for (int i = 0, pixels = 0; i < predictionErrorHistogram.size (); i++) 
	{
		out << predictionErrorHistogram[i] << endl;
	}
	out.close ();

	return 0;
}

// calculate the log base 2 of a number
double log2 (double x) 
{
	if (x == 0) return 0;
	return log (x) / (log (2.0) );
}

// snake raster scan function - updates x and y for a raster-snake scan
// of image, returning false only when the whole image has been traversed
bool SnakePixel (const Image& image, int& x, int& y) 
{
	if (y % 2 == 0) 
	{
		x++;
		if (x == image.Width () ) 
		{
			x--;
			y++;
		}
	}
	else 
	{
		x--;
		if (x == -1) 
		{
			x++;
			y++;
		}		
	}
	return (y < image.Height () );
}

