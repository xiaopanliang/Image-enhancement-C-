/*===
fft.cc

Abstract: Contains definitions for one-dimensional FFT functions.
===*/

#include "pch.h"
#include <math.h>
#include "fft.h"
#include <iostream>
# define M_PI          3.141592653589793238462643383279502884
using namespace std;

/*
====
Complex UnityRoot (int N, int m)

Returns cis(2*pi*m/N), or the Nth root of unity, raised to the m
====
*/
Complex FFT::UnityRoot (int N, int m) {
	m %= N;
	float angle = 2 * M_PI * (double)m / (double)N;
	return Complex (cos(angle), sin(angle));
}

/*
====
vector<Complex> DITimeFFT (vector<Complex>& sequence)

Returns the DFT of the input sequence.  If the sequence does
not have a power-of-two length the output is undefined.
====
*/
void FFT::DITimeFFT (vector<Complex>& sequence) {
	int N = sequence.size ();

	if (N <= 1) {
		return;
	}
	else {
		// divide the input
		vector<Complex> oddSequence (N/2);
		vector<Complex> evenSequence (N/2);
		for (int m = 0; m < N/2; ++m) {
			evenSequence[m] = sequence[2*m];
			oddSequence[m] = sequence[2*m + 1];
		}

		// recurse
		DITimeFFT (evenSequence);
		DITimeFFT (oddSequence);

		// reassemble the output
		for (int m = 0; m < N/2; ++m) {
			sequence[m] = evenSequence[m] + UnityRoot (N, -m)*oddSequence[m];
			sequence[m + N/2] = evenSequence[m] - UnityRoot (N, -m)*oddSequence[m];
		}
	}
}

/*
void InverseTransform (vector<Complex>&)

Zero-pads the sequence to a power of two, computes the inverse FFT of the 
resulting sequence, and returns it.  The output is not guaranteed to be the 
same length as the input.
*/
void FFT::InverseTransform (vector<Complex>& sequence) {
	// calculate the FFT of the input sequence
	vector<Complex> sequenceFFT (sequence);
	Transform (sequenceFFT);
	int N = sequenceFFT.size ();

	// reverse and scale the result
	sequence.resize (N);
	for (int i = 0; i < N; ++i) {
		sequence[i] = sequenceFFT[(N-i) % N] / (double) N;
	}
}

/*
====
void FFT (vector<Complex>& sequence)
====
*/
void FFT::Transform (vector<Complex>& sequence) {
	// zero-pad the input to a power of two
	int n = (int)(log ((double)sequence.size () )/log ((double)2) + 0.5);
	int N = (int)pow (2.0, (double)n);
	sequence.resize (N, Complex(0,0) );

	// call the recursive FFT function
	DITimeFFT (sequence);
}
