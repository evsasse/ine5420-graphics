#ifndef STRUCTS_H
#define STRUCTS_H

#include <gtkmm.h>

#include <math.h>

struct ObjectColumnRecord : public Gtk::TreeModelColumnRecord
{
    ObjectColumnRecord() {
        add(col_Name);
        add(col_Type);
    }

    Gtk::TreeModelColumn<Glib::ustring> col_Name;
    Gtk::TreeModelColumn<Glib::ustring> col_Type;
};

struct Rectangle
{
	Rectangle() : 
		xMin(0), yMin(0), xMax(0), yMax(0) {}

	Rectangle(double xMin, double yMin, double xMax, double yMax) : 
		xMin(xMin), yMin(yMin), xMax(xMax), yMax(yMax) {}

	double xMin, yMin, xMax, yMax; 		
};

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

    static Matrix rotation(double thetaDegrees) {
    	Matrix m;
    	m.v[0][0] = cos(thetaDegrees*M_PI/180);
    	m.v[0][1] = -sin(thetaDegrees*M_PI/180);
    	m.v[1][0] = sin(thetaDegrees*M_PI/180);
    	m.v[1][1] = cos(thetaDegrees*M_PI/180);
		return m;
    }

	double v[3][3];
};

#endif