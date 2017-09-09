#ifndef DISPLAYFILE_H_
#define DISPLAYFILE_H_

#include <vector>

#include "drawable.h"

struct DisplayFile {
	std::vector<Drawable*> drawables;
};

#endif /* DISPLAYFILE_H_ */
