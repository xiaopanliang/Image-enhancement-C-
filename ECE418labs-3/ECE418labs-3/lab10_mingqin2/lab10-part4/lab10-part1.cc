/*===
lab10-part1-solution.cc

Abstract: Skeleton for part one of lab ten (entropy calculation)
===*/

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "image.h"
using namespace std;

// calculate the log base 2 of a number, using the conventation that log2(0)=0
double log2 (double x) 
{
	if (x == 0) return 0;
	return log (x) / (log (2.0) );
}

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
	
	// create the normalized (sum of elements is one) histogram for image
	vector<double> histogram (256, 0.0);
	for (int x = 0; x < image.Width (); x++)
	{
		for (int y = 0; y < image.Height (); y++)
		{
			histogram[image.Pixel (x, y)] += 1.0;
		}
	}
	
	// calculate and print the entropy of the histogram
	double entropy = 0;

	// a. calculate the probabilities
	vector<double> probofi (256, 0.0);
	for (int x = 0; x < 256 ; x++)
	{
		probofi[x] = histogram[x]/(512*512);
	}

	// b. calculate the entropy
	for (int x = 0; x<256 ; x++)
	{
		entropy = entropy-probofi[x]*log2(probofi[x]);
	}

	// c. calculate total number of bits
	double totalbits = 0;
	double length;
	double width;
	length = image.Height ();
	width = image.Width ();
	totalbits = entropy*length*width;
	
	// d. dispy entropy and total number bits on the screen
	cout << "Entropy = " << entropy << endl;
	cout << "The total number of bits requires is " << totalbits <<endl;

	// print the histogram the specified file
	ofstream out (argv[2]);
	if (out.fail () )
	{
		cerr << "Failed to open file for histogram output" << endl;
		return 1;
	}	
	for (int i = 0; i < histogram.size (); i++)
	{
		out << histogram[i] << endl;
	}
	out.close ();

	return 0;
}

