#include <stdlib.h>
#include <iostream>
#include <vector>
#include "pch.h"
#include "image.h"
#include "entropy.h"
#include "RunLengthCoding.h"
#include "PredictiveCoding.h"
using namespace std;

int main(int argc, char* argv[])
{
	// check parameters' correctness [for parts (a) and (b) -- this needs to be changed for (c)]
	if (argc != 5)
	{
		cerr << "Usage: " << argv[0] << " in.png out1.png out2.png out3.png" << endl;
		return 1;
	}

	entropy(argv[1], argv[2]);
	RunLengthCoding(argv[1], argv[3]);
	PredictiveCoding(argv[1], argv[4]);
}