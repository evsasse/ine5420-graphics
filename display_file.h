#ifndef DISPLAYFILE_H_
#define DISPLAYFILE_H_

#include <vector>

#include "drawable.h"

struct DisplayFile {
	std::vector<Point> points;
	std::vector<Line> lines;
	std::vector<Wireframe> wireframes;
};

#endif /* DISPLAYFILE_H_ */
