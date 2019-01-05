#include <stdlib.h>
#include <iostream>
#include <vector>
#include "pch.h"
#include "image.h"
#include "ImageHistogram.h"
#include "HistogramEqual.h"
using namespace std;

int main(int argc, char* argv[])
{
	// check parameters' correctness [for parts (a) and (b) -- this needs to be changed for (c)]
	if (argc != 5)
	{
		cerr << "Usage: " << argv[0] << " in.png out1.png out2.png [bincount] " << endl;
		return 1;
	}

	int binCount = atoi(argv[4]);
	ImageHistogram(argv[1], argv[2], binCount);
	HistogramEqual(argv[1], argv[3]);
}