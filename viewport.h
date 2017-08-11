#ifndef VIEWPORT
#define VIEWPORT

#include <vector>

#include "drawable.h"

using namespace std;

class Viewport {
public:
	Viewport() :
		x(0), y(0), scale(1)
		{};

	double x, y, scale;
	vector<Point> drawables;
};

#endif