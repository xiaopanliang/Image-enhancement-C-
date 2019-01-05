/*===
lab10-part2-solution.cc

Abstract: Skeleton for part two of lab ten (run length coding)
===*/

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "image.h"
using namespace std;

// calculate the log base 2 of a number
double log2 (double x);

// snake raster scan function -- updates x and y for a raster-snake scan
// of image, returning false only when the whole image has been traversed
bool SnakePixel (const Image& image, int& x, int& y);

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
	
	// create the normalized (sum of elements is one) histogram for the run lengths
	vector<double> runLengthHistogram (totalPixels + 1, 0);

	// a. Save pixel values in vector pixelvaluevector
	vector<double> pixelvaluevector(totalPixels,0);
	int apixel = 1;
	pixelvaluevector[0] = image.Pixel(0,0);
	for (int x = 0, y = 0; SnakePixel (image, x, y); )
	{
		pixelvaluevector[apixel] = image.Pixel(x,y);
		apixel = apixel + 1;
	}
	//cout << "Run-Length Entropy is " << pixelvaluevector[1]<< "." << endl;

	// b. calculate the quadruple string (run length string)
	// and calculate the total number of runs at the same time
	int runlength = 1;
	double runtime = 1.0;
	vector<double> quadruple(totalPixels,0);
	for (int i = 1; i< totalPixels+1; i++)
	{
		if(pixelvaluevector[i] == pixelvaluevector[i-1])
		{
			if(i != totalPixels )
			{
			runlength = runlength + 1;
			}
			else
			{
			quadruple[i] = runlength + 1;
			}
		}
		else
		{
			quadruple[i-1] = runlength;
			runlength = 1;
			runtime = runtime + 1.0;
		}
	}

	// c. calculate the histogram corresponding to the run length
	int b;
	for (int i = 0; i< totalPixels+1; i++)
	{
		if(quadruple[i] != 0 )
		{
			b = quadruple[i];
			runLengthHistogram[b] = runLengthHistogram[b] + 1;
		}
	}
	
	// d. calculate and print the entropy of the histogram
	double entropy = 0;

	// d1. calculate the probabilities
	vector<double> probofi (runLengthHistogram.size(), 0.0);
	for (int i = 0; i < runLengthHistogram.size (); i++)
	{
		probofi[i] = runLengthHistogram[i]/(runtime);
		/*===

		Insert Lab 10 Part 2 Code Here
		
		===*/
	}
		// Verify the summation of probabilities is 1
	double a = 0;
	for (int i = 0; i < runLengthHistogram.size (); i++)
	{
		a = probofi[i] + a;
	}
	//cout << "Summation of probabilities is " << a << "." << endl;

	// d2. calculate the entropy
	for (int i = 0; i < runLengthHistogram.size (); i++)
	{
		entropy = entropy-probofi[i]*log2(probofi[i]);
	}
	cout << "Run-Length Entropy is " << entropy << "." << endl;

	// d3. print other requested data
	double totalbits = entropy*runtime;
	double bitperpixel = totalbits/(512.0*512.0);
	cout << "The total number of runs is " << runtime << "." << endl;
	cout << "The total number of bits requires is " << totalbits << " bits." << endl;
	cout << "The number of bits per pixel requires is " << bitperpixel << " bits." << endl;

	// print the histogram the specified file (only as many entries as are nonzero)
	ofstream out (argv[2]);
	if (out.fail () ) 
	{
		cerr << "Failed to open file for histogram output" << endl;
		return 1;
	}
	int maxNonZeroIndex = 0;
	for (int i = 0; i < runLengthHistogram.size (); i++) 
	{
		if (runLengthHistogram[i] != 0) {
			maxNonZeroIndex = i;
		}
	}
	for (int i = 0, pixels = 0; i <= maxNonZeroIndex; i++) 
	{
		out << runLengthHistogram[i] << endl;
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

// snake raster scan function -- updates x and y for a raster-snake scan
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

