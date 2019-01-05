/*===
fftimage.h

Abstract: Contains declaration for image class with a built-in and efficient 2D FFT
===*/

#ifndef _IMAGEFFT_H_
#define _IMAGEFFT_H_

#include "image.h" // for Complex and ComplexImage types

/*===
ComplexFFTImage
The base image class.  Because in this course we will use images whose pixels have complex values
   as well as images whose pixels only have integer values, the type of pixel we use is templated.
===*/
class ComplexFFTImage : public ComplexImage
{
public:
	void FourierTransform();
	void InverseFourierTransform();

private:
	/*=
	Fourier transforms A, treating it as an array of n values, separated by stride.
	  This way, if we want to transform the ith column of an image's one-dimensional array
	  A, we can set the stride to be the width of the image, so the call would be
	  FFT (&A[i], height, width).  For the ith row, the call would be
	  FFT (&A[i*width], width, 1)
	=*/
	static void FFT(Complex* A, int n, int stride);

	// helper functions described in fftimage.cc or CLRS
	static Complex UnityRoot(int N, int m);
	static void Swap(Complex& a, Complex& b);
	static int ChopLg(int n);
	static int BitReverse(unsigned int k, int N, int lgN);
	static void BitReverse(Complex* A, int N, int lgN, int stride);
};

#endif