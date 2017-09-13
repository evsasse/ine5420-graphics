#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <gtkmm.h>

struct Viewport {
	Viewport(double x, double y, double borderSize) {
        setViewportSize(x, y, borderSize);
	}

    void setViewportSize(double x, double y, double borderSize) {
        xMin = borderSize;
        xMax = x - borderSize;
        yMin = borderSize;
        yMax = y - borderSize;
    }

    double xMin, xMax, yMin, yMax;

    double xSize() const{
        return xMax - xMin;
    }

    double ySize() const{
        return yMax - yMin;
    }
};

#endif