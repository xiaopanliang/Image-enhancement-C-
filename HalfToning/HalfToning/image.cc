/*===
image.cc

Abstract: Contains the implementation of functions for the ImageTemplate class, defined in 
image-template.h.
===*/

#include "pch.h"
#include "image.h"

// Template specialization for a complex pixel.
template<>
unsigned char GreyValue(Complex pixelValue)
{
  return static_cast<unsigned char>(pixelValue.real());
}
