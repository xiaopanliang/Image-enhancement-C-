
#include "pch.h"
#include "LowPassFilter.h"
#include "ZeroPhase.h"
#include "MiniMagnitudeVariation.h"
#include "image.h"
#include "fft.h"
#include <stdlib.h>
#include <iostream>
#include <vector>

using namespace std;

// The first output is zero phase image
// The second output is zero magnitude variation image
// The third output is low-pass filtered image
int main(int argc, char* argv[])
{
	if (argc != 6)
	{
		cerr << "Usage: " << argv[0] << " in.png out1.png out2.png out3.png frequency" << endl;
		return 1;
	}
	double frequency = atof(argv[5]);
	if (frequency < 0 || frequency > 0.5)
	{
		cerr << "Cutoff frequency must be in [0,0.5]" << endl;
		return 1;
	}
	ZeroPhase(argv[1], argv[2]);

	MiniMagnitudeVariation(argv[1], argv[3]);

	LowPassFilter(argv[1], argv[4], frequency);

	return 0;
}
