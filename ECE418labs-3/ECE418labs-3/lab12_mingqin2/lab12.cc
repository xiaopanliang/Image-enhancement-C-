/*===
lab12-skeleton.cc

Abstract: Skeleton code for lab 12
===*/

#include "image.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

// main
int main (int argc, char* argv[]) {
	
	// check arguments' correctness
	if (argc != 8) {
		cout << "Usage : " << argv[0] 
		     << " current previous blocksize"
		     << " xblock yblock search_size output_ascii_file" 
		     << endl;
	  return 1;
	}

	// load the current frame
	Image current;
	current.LoadPng ( argv[1] );

	// load the previous frame
	Image previous;
	previous.LoadPng ( argv[2] );

	// parse command line arguments
	int blockSize = atoi(argv[3]);  // the size of the block we will be comparing
	int xStart = atoi(argv[4]);     // starting horizontal pixel for the block we will compare
	int yStart = atoi(argv[5]);     // starting vertical pixel for the block we will compare
	int searchSize = atoi(argv[6]); // number of pixels up and down our search will encompass
	
	// initialize the 2D array holding the MSE for each displacement to have size searchSize^2
	//vector<vector<double> > displacementMSE (searchSize, vector<double> (searchSize, 0) );
	double displacementMSE[searchSize][searchSize];
	// fill in the displacementMSE table and print the displacement that minimizes MSE
	/*===
	
	Insert Lab 12 Code Here
	
	===*/
	//Declaring
	int vmin = -(searchSize-1)/2;
	int vmax = (searchSize+1)/2;
	double pixelcurrent;
	double pixelprevious;
	Image difference;
	difference.Resize (16,16);
	double differ;
	double minMSE = 100.0;
	int optimali;
	int optimalj;
	
	// For every possible motion vector <i,j> 
	// Range from -31 to 31
	for (int i = vmin; i< vmax; i++)
	{
		for (int j = vmin; j<vmax; j++)
		{
			// For every 16x16 block
			for(int blocki = 0; blocki < 16; blocki++)
			{
				for(int blockj = 0; blockj < 16; blockj++)
				{
				// get the pixel value for the current pixel in a block
				pixelcurrent = current.Pixel(xStart+blocki,yStart+blockj);
				// get the pixel value for the previous pixel in the displacement block
				// corresponding to the motion vector
				pixelprevious = previous.Pixel(xStart+blocki+i,yStart+blockj+j);
				// save their difference to a 16x16 difference image
				difference.Pixel(blocki,blockj) = pixelcurrent - pixelprevious;
				}
			}
			// Calculate the MSE for the block
			double MSEblock = 0.0;
			for (int a = 0; a < 16; a++)
			{
				for (int b = 0; b < 16; b++)
				{
				differ = difference.Pixel(a,b);
				MSEblock = MSEblock + pow(differ,2);
				}
			}
			MSEblock = MSEblock/16.0/16.0;
			// Save MSE to 2D array displacementMSE corresponding to motion vector <i,j>
			displacementMSE[i+31][j+31]=MSEblock;
			// Find the minimum MSE value and corresponding motion vector
			if (MSEblock < minMSE)
			{
				minMSE = MSEblock;
				optimali = i;
				optimalj = j;
			}
		}
	}
	// minimum MSE and corresponding motion vector
	cout<< "minMSE = " << minMSE << endl;
	cout<< "displacement vector with minimum MSE is " << "("<<optimali<<","<<optimalj<<")."<< endl;

	// save each displacement's MSE to a file
	ofstream out (argv[7]);
	if (out.fail () ) 
	{
		cout << "Failed to open " << argv[7] << " for writing." << endl;
		return 1;
	}	
	for (int x = vmin; x < vmax; x++)
	{
		for (int y = vmin; y < vmax; y++)
		{
			out << x << " " << y << " " << displacementMSE[x+31][y+31] << endl;
		}
	}
	out.close ();	
}
