#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "pch.h"
#include "image.h"
using namespace std;

double log2(double x)
{
	if (x == 0) return 0;
	return log(x) / (log(2.0));
}