#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "pch.h"
#include "image.h"
using namespace std;

bool SnakePixel(const Image& image, int& x, int& y)
{
	if (y % 2 == 0)
	{
		x++;
		if (x == image.Width())
		{
			x--;
			y++;
		}
	}
	else
	{
		x--;
		if (x == -1)
		{
			x++;
			y++;
		}
	}
	return (y < image.Height());
}