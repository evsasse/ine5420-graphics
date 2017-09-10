#ifndef WINDOW_H_
#define WINDOW_H_

#include "display_file.h"

/*
+=========WORLD==========+
|                        |
|   +=====WINDOW=====+   |
|   |                |   |
|   |     v △        |   |
|   |       |        |   |
|   |   <---+--->    |   |
| y |       |   u    |   |
| △ |       ▽        |   |
| | |                |   |
| | +================+   |
| +---> x                |
+========================+

The world space is described by the x and y coordinates.

The window space is described by the u and v coordinates.

The window space has its origin in the center of the window
and the drawable is from "minus one" to "one" in each direction.

Each direction may have have different world sizes, because
the window doesnt need to be square.

The coordinates in the world need to be calculated into the
window space so we can know when its necessary to draw them.

The window may move, rotate, and scale, as an object in the world,
therefore its axis are independent from the world's axis.
*/

struct Window {
	Window(double x_center, double y_center, double u_base, double v_base) : 
		x_center(x_center), y_center(y_center), u_base(u_base), v_base(v_base),
		scale(1), rotation(0) {} 

	// dictates the world size of "one u"
	double u_size() const {
		return u_base * scale;
	}

	// dictates the world size of "one v"
	double v_size() const {
		return v_base * scale;
	}

	// the position of the window in the world
	double x_center, y_center;

	// describes the proportion of the window
	// should be based on the viewport size, not to cause visual stretch
	double u_base, v_base;	

	// "zoom" 
	double scale;	

	// the rotation of the window in the world
	double rotation;
};

#endif /* WINDOW_H_ */
