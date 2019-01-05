/*===
image.h

Abstract: Contains the defintion of the the following types: ImageTemplate, Image, ComplexImage, Complex
===*/

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <png.h>

#include <complex>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

/*===
ImageTemplate<PixelType>
The base image class.  Because in this course we will use images whose pixels have complex values
   as well as images whose pixels only have integer values, the type of pixel we use is templated.
===*/
template<typename PixelType>
class ImageTemplate
{
public:
	// Constructors & destructors.  The default constructor creates a 0*0 image.
	ImageTemplate();
	~ImageTemplate();
	ImageTemplate(const ImageTemplate&);
	const ImageTemplate& operator= (const ImageTemplate&);

	// Returns the value of the image at the specified coordinate.  Invalid coordinates are
	//    handled as per the CorrectionPolicy.  An exception will be thrown if the image width
	//    or height is zero.
	PixelType& Pixel(int x, int y);
	const PixelType& Pixel(int x, int y) const;

	// Image loading and saving functions -- return true on success, false on failure.
	//     On failure of loading functions, the image will be resized to 0*0.
	//     The PNG functions are for use in the labs.  RAW and PGM functions are for
	//     compatibility.
	//     [ The saving functions require the existence of a function 
	//     char GreyValue (PixelType) in order to link ]
	bool LoadPng(std::string fileName);
	bool SavePng(std::string fileName) const;

	bool LoadRaw(std::string fileName, int width, int height);
	bool SaveRaw(std::string fileName) const;
	bool SavePgm(std::string fileName) const;

	// Get the width or height of the image
	int Width() const;
	int Height() const;

	// Resize the image
	void Resize(int width, int height);

protected:
	// Returns the index of the coordinate in the m_data array.  This implements the pixel
	//    correction policy
	int CoordinateIndex(int x, int y) const;

	// The data of the image.  It is stored in a one-dimensional array so that the FFT function
	//    is faster in ComplexFFTImage.  This is not allocated and set to NULL if _width or _height 
	//    is zero.
	PixelType* _data;

	// Image dimensions.
	int _width;
	int _height;
};

// Image class typedef for integer-valued images
typedef ImageTemplate<int> Image;

// ComplexImage class for complex-valued images
typedef std::complex<double> Complex;
typedef ImageTemplate<Complex> ComplexImage;

// convert from a PixelType to a grey value in [0,255] for writing to files
template<typename PixelType>
unsigned char GreyValue(PixelType pixelValue)
{
	return static_cast<unsigned char>(pixelValue);
}

// Complex GreyValue template specialization declaration.
template<>
unsigned char GreyValue(Complex pixelValue);

// Default constructor
template<typename PixelType>
ImageTemplate<PixelType>::ImageTemplate() :
	_width(0),
	_height(0),
	_data(NULL)
{
}

// Destructinator
template<typename PixelType>
ImageTemplate<PixelType>::~ImageTemplate()
{
	Resize(0, 0); // this will delete m_data and set it to NULL (not re-allocate it)
}

// Copy constructor
template<typename PixelType>
ImageTemplate<PixelType>::ImageTemplate(const ImageTemplate<PixelType>& original) :
	_width(0),
	_height(0),
	_data(NULL)
{
	*this = original;
}

// Equals operator overload
template<typename PixelType>
const ImageTemplate<PixelType>& ImageTemplate<PixelType>::operator= (const ImageTemplate<PixelType>& original)
{
	Resize(original.Width(), original.Height());
	for (int x = 0; x < _width; x++)
	{
		for (int y = 0; y < _height; y++)
		{
			Pixel(x, y) = original.Pixel(x, y);
		}
	}
	return *this;
}

// Pixel access
template<typename PixelType>
PixelType& ImageTemplate<PixelType>::Pixel(int x, int y)
{
	return _data[CoordinateIndex(x, y)];
}

template<typename PixelType>
const PixelType& ImageTemplate<PixelType>::Pixel(int x, int y) const
{
	return _data[CoordinateIndex(x, y)];
}

// LoadPng
// The I/O code in this file was originally based on the example.c skeleton code
// distributed with the PNG library, and modified by Michael Garland in libgfx.
template<typename PixelType>
bool ImageTemplate<PixelType>::LoadPng(std::string filename)
{
	FILE *fp = fopen(filename.c_str(), "rb");
	if (!fp)
	{
		return false;
	}

	// The last three arguments can be used to set up error handling callbacks.
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		fclose(fp);
		return false;
	}

	// Allocate required structure to hold memory information.
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		fclose(fp);
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return false;
	}

	// Because we didn't set up any error handlers, we need to be
	// prepared to handle longjmps out of the library on error
	// conditions.
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		return false;
	}

	png_init_io(png_ptr, fp);

	// Read in all the image information
	png_read_info(png_ptr, info_ptr);

	// Get the header for the first image chunk
	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type;
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, NULL, NULL);

	// truncate 16 bits/pixel to 8 bits/pixel
	if (bit_depth == 16)
	{
		png_set_strip_16(png_ptr);
	}

	// expand paletted colors into RGB color values
	if (color_type == PNG_COLOR_TYPE_PALETTE)
	{
		png_set_expand(png_ptr);
	}

	// expand grayscale images to full 8 bits/pixel
	else if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
	{
		png_set_expand(png_ptr);
	}

	// Expand paletted or RGB images with transparency to full alpha
	// channels so the data will be available as RGBA quartets.
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
	{
		png_set_expand(png_ptr);
	}

	// update the palette and info structure
	png_read_update_info(png_ptr, info_ptr);

	// read the image data
	std::vector<png_bytep> row_pointers(height);
	int nchan = png_get_channels(png_ptr, info_ptr);
	int nbytes = png_get_rowbytes(png_ptr, info_ptr);
	for (int row = 0; row < height; row++)
	{
		row_pointers[row] = (png_bytep)malloc(nbytes);
	}
	png_read_image(png_ptr, &row_pointers.front());
	png_read_end(png_ptr, info_ptr);

	// warn if the image has more channels than we can handle
	if (nchan != 1)
	{
		std::cout << "Warning: " << filename << " has more than one channel (only reading the first)." << std::endl;
	}

	// read the data and convert its pixel type
	Resize(width, height);
	for (int row = 0; row < _height; row++)
	{
		for (int column = 0; column < _width; column++)
		{
			Pixel(column, row) = PixelType(row_pointers[row][column*nchan]);
		}
	}

	// destroy the data and close the file
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	for (int row = 0; row < height; row++)
	{
		free(row_pointers[row]);
	}
	fclose(fp);
	return true;
}

// SavePng
// See comments from LoadPng
template<typename PixelType>
bool ImageTemplate<PixelType>::SavePng(std::string filename) const
{
	FILE *fp = fopen(filename.c_str(), "wb");
	if (!fp)
	{
		return false;
	}

	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		fclose(fp);
		return false;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		fclose(fp);
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		return false;
	}

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		fclose(fp);
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		return false;
	}

	png_init_io(png_ptr, fp);

	int img_type = PNG_COLOR_TYPE_GRAY;

	png_set_IHDR(
		png_ptr, info_ptr,
		_width, _height, 8, img_type, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE
	);

	png_write_info(png_ptr, info_ptr);

	// convert the data to bytes so that we can pass it to the png writer
	unsigned char* bytePixelData = new unsigned char[_width*_height];
	for (int i = 0; i < _width*_height; i++)
	{
		bytePixelData[i] = GreyValue(_data[i]);
	}

	std::vector<png_bytep> row_pointers(_height);
	for (int k = 0; k < _height; k++)
	{
		row_pointers[k] = (png_bytep)&bytePixelData[k * _width];
	}

	png_write_image(png_ptr, &row_pointers.front());
	png_write_end(png_ptr, info_ptr);

	png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	fclose(fp);
	delete[] bytePixelData;
	return true;
}

// SavePgm
template<typename PixelType>
bool ImageTemplate<PixelType>::SavePgm(std::string fileName) const
{
	std::ofstream out;
	out.open(fileName.c_str(), std::ios::binary);
	if (out.fail())
	{
		std::cerr << "Failed to open " << fileName << " for writing." << std::endl;
		return false;
	}

	out << "P5" << std::endl << _width << " " << _height << std::endl << 255 << std::endl;
	for (int i = 0; i < _width*_height; i++)
	{
		unsigned char byte = GreyValue(_data[i]);
		out.write((char*)&byte, 1);
	}
	out.close();
	return true;
}

// LoadRaw
template<typename PixelType>
bool ImageTemplate<PixelType>::LoadRaw(std::string fileName, int width, int height)
{
	Resize(width, height);

	std::ifstream in;
	in.open(fileName.c_str(), std::ios::binary);
	if (in.fail())
	{
		std::cerr << "Failed to open " << fileName << " for reading." << std::endl;
		Resize(0, 0);
		return false;
	}
	for (int i = 0; i < width*height; i++)
	{
		unsigned char byte;
		in.read((char*)&byte, 1);
		_data[i] = PixelType(byte);
	}
	in.close();
	return true;
}

// SaveRaw
template<typename PixelType>
bool ImageTemplate<PixelType>::SaveRaw(std::string fileName) const
{
	std::ofstream out;
	out.open(fileName.c_str(), std::ios::binary);
	if (out.fail())
	{
		std::cerr << "Failed to open " << fileName << " for writing." << std::endl;
		return false;
	}

	for (int i = 0; i < _width*_height; i++)
	{
		char byte = GreyValue(_data[i]);
		out.write((char*)&byte, 1);
	}
	out.close();
	return true;
}

// Width and Height accessors
template<typename PixelType>
int ImageTemplate<PixelType>::Width() const
{
	return _width;
}

template<typename PixelType>
int ImageTemplate<PixelType>::Height() const
{
	return _height;
}

// Resize
template<typename PixelType>
void ImageTemplate<PixelType>::Resize(int width, int height)
{
	_width = _height = 0;
	delete[] _data; // calling delete on NULL is kosher (see Soustroup)
	_data = NULL;

	if (width < 0 || height < 0)
	{
		std::cerr << "Image dimensions (" << width << "," << height << ") are invalid." << std::endl;
		throw ("Illegal image dimensions");
	}
	_width = width;
	_height = height;

	int numberOfPixels = _width * _height;
	_data = new PixelType[numberOfPixels]; // if numberOfPixels == 0, this will return NULL (again, see Soustroup)

	if (numberOfPixels != 0 && _data == NULL)
	{
		std::cerr << "Could not allocate memory for image" << std::endl;
		throw ("Could not allocate memory for image");
	}
}

// Resize
template<typename PixelType>
int ImageTemplate<PixelType>::CoordinateIndex(int x, int y) const
{
	// make sure the image is not vacuous
	if (false == (_width || _height))
	{
		std::cerr << "Cannot access a pixel in an empty image" << std::endl;
		throw ("Cannot access a pixel in an emtpy image");
	}

	// warn if we were told to try to access an invalid pixel
	if (x < 0 || x >= _width || y < 0 || y >= _height)
	{
		std::cerr << "Warning: The pixel coordinate (" << x << "," << y << ") is invalid, clamping coordinates to the image's dimensions" << std::endl;
	}

	x = std::min(x, _width - 1);
	y = std::min(y, _height - 1);
	x = std::max(x, 0);
	y = std::max(y, 0);

	return (y*_width + x);
}


#endif