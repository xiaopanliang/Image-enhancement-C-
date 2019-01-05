#include <stdlib.h>
#include <iostream>
#include <vector>
#include "pch.h"
#include "image.h"
#include "imagefft.h"
#include "interpolation.h"
#include "Bilinear_inter.h"
#include "decimation.h"
using namespace std;

int main(int argc, char* argv[])
{
	// check parameters' correctness [for parts (a) and (b) -- this needs to be changed for (c)]
	if (argc != 11)
	{
		cerr << "Usage: " << argv[0] << " in.png restored.png diff_image.png out1.png out2.png out3.png [top] [left] [factor] [flag]" << endl;
		return 1;
	}

	// read the top row and left column that will be used [remove this for part (c)]
	int topLeftRow = atoi(argv[7]);
	int topLeftColumn = atoi(argv[8]);
	int factor = atoi(argv[9]);
	int flag = atoi(argv[10]);

	interpolation(argv[1], argv[4], topLeftRow, topLeftColumn);
	Bilinear_inter(argv[1], argv[5], topLeftRow, topLeftColumn);
	decimation(argv[1], argv[2], argv[3], argv[6], factor, flag);
}