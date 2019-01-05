#include <stdlib.h>
#include <iostream>
#include <vector>
#include "pch.h"
#include "image.h"
#include "dither.h"
#include "dither4X4.h"
#include "errdiff.h"
using namespace std;

int main(int argc, char* argv[])
{
	// check parameters' correctness [for parts (a) and (b) -- this needs to be changed for (c)]
	if (argc != 5)
	{
		cerr << "Usage: " << argv[0] << " in.png out1.png out2.png out3.png " << endl;
		return 1;
	}

	dither(argv[1], argv[2]);
	dither4X4(argv[1], argv[3]);
	errdiff(argv[1], argv[4]);
}