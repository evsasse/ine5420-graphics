#ifndef STRUCTS_H
#define STRUCTS_H

#include <gtkmm.h>

#include <math.h>

#include "sgiwindow.h"

struct Matrix {
	Matrix() {
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				v[i][j] = (i == j ? 1 : 0);				
			}
		} 
	}

	static Matrix translation(double dx, double dy) {
		Matrix m;
		m.v[2][0] = dx;
		m.v[2][1] = dy;
		return m;
	}

    static Matrix scaling(double cx, double cy, double sx, double sy) {
    	Matrix m;
		m.v[0][0] = sx;
	    m.v[1][1] = sy;
	    m.v[2][2] = 1;
	    m.v[2][0] = - cx * sx + cx;
	    m.v[2][1] = - cy * sy + cy;
		return m;
    }

    static Matrix rotation(double dx, double dy, double degrees) {
    	double radians = degrees*M_PI/180;
    	Matrix m;    	
    	m.v[0][0] = cos(radians);
    	m.v[0][1] = -sin(radians);
    	m.v[1][0] = sin(radians);
    	m.v[1][1] = cos(radians);
    	m.v[2][0] = -dx * cos(radians) - dy * sin(radians) + dx;
    	m.v[2][1] = dx * sin(radians) - dy * cos(radians) + dy;
		return m;
    }

    static Matrix window_transformation(const SGIWindow &w) {
    	double radians = w.rotation*M_PI/180;
    	Matrix m;
    	m.v[0][0] = cos(-radians) / w.u_size();
    	m.v[0][1] = -sin(-radians) / w.v_size();
    	m.v[1][0] = sin(-radians) / w.u_size();
    	m.v[1][1] = cos(-radians) / w.v_size();
    	m.v[2][0] = (-w.x_center * cos(-radians) - w.y_center * sin(-radians)) / w.u_size();
    	m.v[2][1] = (w.x_center * sin(-radians) - w.y_center * cos(-radians)) / w.v_size();
    	return m;
    }

	double v[3][3];
};

#endif