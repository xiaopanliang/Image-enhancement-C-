/*===
imagefft.cc

Abstract: Contains implementation of image class with efficient 2D DFT
===*/


#include "pch.h"
#include "image.h"
#include "imagefft.h"
#define M_PI          3.141592653589793238462643383279502884


void ComplexFFTImage::FourierTransform () 
{
	// transform the columns
	for (int i = 0; i < _width; i++) 
	{
		ComplexFFTImage::FFT (&_data[i], _height, _width);
	}
	// transform the row
	for (int j = 0; j < _height; j++) 
	{
		ComplexFFTImage::FFT (&_data[j*_width], _width, 1);
	}
}

void ComplexFFTImage::InverseFourierTransform () 
{
	// perform the forward FFT
	FourierTransform ();

	// reverse and scale the image
	int widthTimesHeight = _width*_height;
	for (int i = 0; i < _width; i++) 
	{
		for (int j = 0; j < _height; j++) 
		{
			int modNegativei = (_width - i) % _width;
			int modNegativej = (_height - j) % _height;

			if (i < modNegativei || (i == modNegativei && j < modNegativej) ) 
			{
				Swap ( _data[j*_width + i], _data[modNegativej*_width + modNegativei]);
			}

			_data[j*_width + i] /= widthTimesHeight;
		}
	}
}

/*
====
Complex UnityRoot (int N, int m)

Returns cis(2*pi*m/N), or the Nth root of unity, raised to the m
====
*/
Complex ComplexFFTImage::UnityRoot (int N, int m) 
{
	m %= N;
	float angle = 2 * M_PI * (double)m / (double)N;
	return Complex (cos(angle), sin(angle));
}

/*
====
void Swap (Complex& a, Complex& b)

Swaps the values of a and b
====
*/
void ComplexFFTImage::Swap (Complex& a, Complex& b)
{
	static Complex temp;
	temp = a;
	a = b;
	b = temp;
}


/*
====
int ChopLg (int n)

returns the chop (round-down) of log base 2 of n, denoted lg n
====
*/
int ComplexFFTImage::ChopLg (int n) 
{
	int lgn = -1;
	while (n > 0) 
	{
		n /= 2;
		++lgn;
	}
	return lgn;
}

/*
====
int BitReverse (unsigned int k, int N)

Returns k, with the least significant N bits reversed
====
*/
int ComplexFFTImage::BitReverse (unsigned int k, int N, int lgN) 
{
	unsigned int rev = 0;
	for (int i = 0; i < lgN; i++) 
	{
		if ( k & (1 << i) ) 
		{
			rev += N/2 >> i;
		}
	}
	return rev;
}

/*
====
void BitReverseArray (Complex* A, int N, int stride)

Bit-reverses the elements of A.  For example, if A has length 8
then elements 4 and 1 (100 and 001) are swapped (and so on, so forth)
====
*/
void ComplexFFTImage::BitReverse (Complex* A, int N, int lgN, int stride) 
{
	for (int k = 0; k < N; k++) 
	{
		int kReversed = BitReverse (k, N, lgN);
		if (kReversed > k) 
		{
			Swap ( A[ kReversed*stride ], A[ k*stride ] );
		}
	}
}

/*
====
void FFT (Complex* A, int n, int stride)

This is the implementation of the FFT from the big fat CLRS
algorithms book (section 30.3, page 841 in edition two).  So that
we're all on the same page, I consider this be non-human-readable
====
*/
void ComplexFFTImage::FFT (Complex* A, int n, int stride) 
{
	int lgn = ChopLg (n);
	BitReverse (A, n, lgn, stride);
	for (int s = 0; s <= lgn; s++) 
	{
		int m = (int)pow (2.0, s);
		Complex wm = UnityRoot (m, 1);
		for (int k = 0; k < n; k += m) 
		{
			Complex w = 1.0;
			for (int j = 0; j < m/2; j++) 
			{
				Complex t = w*A[ (k+j+m/2)*stride ];
				Complex u = A[ (k+j)*stride ];
				A[ (k+j)*stride ] = u + t;
				A[ (k+j+m/2)*stride ] = u-t;
				w *= wm;
			}
		}
	}
}
