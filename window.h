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
	DisplayFile *display_file;

	// describes the proportion of the window
	// should be based on the viewport size, not to cause visual stretch
	double u_base, v_base;

	// "zoom" in each axis, should be equal, not to cause visual stretch
	double u_scale, v_scale;

	// these sizes are calculated to, indepently, support zoom, by changing
	// the scales, AND resizing of the viewport, by changing the bases
	// u_base * u_scale => dictates the world size of "one u"
	double u_size() const;
	// v_base * v_scale => dictates the world size of "one v"
	double v_size() const;

	// the position of the window in the world
	double x_center, y_center;

	// the rotation of the window in the world
	double rotation;

};

#endif /* WINDOW_H_ */
