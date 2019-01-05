#pragma once
/*===
fft.h

Abstract: Contains declarations for one-dimensional FFT functions.
===*/

#ifndef _FFT_H_
#define _FFT_H_

#include "image.h" // for Complex type
#include <vector>

/*===
FFT
A static class for one-dimensional FFT functions.
===*/
class FFT
{
public:
	static void Transform(std::vector<Complex>&);        // Fourier transforms the supplied vector
	static void InverseTransform(std::vector<Complex>&); // Inverse-Fourier transforms the vector
private:
	static Complex UnityRoot(int N, int m);
	static void DITimeFFT(std::vector<Complex>&);        // Recursive decimation in time FFT algorithm
};

#endif