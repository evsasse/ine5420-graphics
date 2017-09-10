#ifndef DISPLAYFILE_H_
#define DISPLAYFILE_H_

#include <vector>

#include "drawable.h"
#include "structs.h"

struct DisplayFile {
	std::vector<Drawable*> drawables;

	void update_window_coordinates(const Matrix &m) {
		for (auto d : drawables) {
			d->setWindowCoordinates(m);
		}
	}
};

#endif /* DISPLAYFILE_H_ */
