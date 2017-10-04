#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <vector>
#include <string>

#include "matrix.h"
#include <cairomm/context.h>

struct Coordinate {

	Coordinate() : x(0), y(0) {}

	Coordinate(double x, double y) :
		x(x), y(y) {}

	double x, y;

	Coordinate applyMatrix(const Matrix &m) const;
};

struct BezierCurve {

	BezierCurve(Coordinate c1, Coordinate c2, Coordinate c3, Coordinate c4) :
		c1(c1), c2(c2), c3(c3), c4(c4) {}

	BezierCurve() :	c1(), c2(), c3(), c4() {}		

	Coordinate c1;
	Coordinate c2;
	Coordinate c3;
	Coordinate c4;

	BezierCurve applyMatrix(const Matrix &m) const;
};

struct Drawable {

	Drawable(std::string name) : 
		name(name) {}

	std::string name;

	virtual std::string type() = 0;
	virtual Coordinate center() = 0;
	virtual void applyMatrix(const Matrix &m) = 0;
	virtual void setWindowCoordinates(const Matrix &m) = 0;

	void translate(double dx, double dy);
	void scale(double sx, double sy);
	void rotate(double deg);
	void rotate(const Coordinate &c, double deg);
};

struct Point : public Drawable {

	Point(std::string name, Coordinate coordinate) :
		Drawable(name),
		coordinate(coordinate) {}

	Coordinate coordinate;
	Coordinate window_coordinate;

	std::string type();
	Coordinate center();
	void applyMatrix(const Matrix &m);
	void setWindowCoordinates(const Matrix &m);
};

struct Line : public Drawable {

	Line(std::string name, Coordinate coordinate_a, Coordinate coordinate_b) :
		Drawable(name),
		coordinate_a(coordinate_a),
		coordinate_b(coordinate_b) {}

	Coordinate coordinate_a, coordinate_b, window_coordinate_a, window_coordinate_b;

	std::string type();
	Coordinate center();
	void applyMatrix(const Matrix &m);
	void setWindowCoordinates(const Matrix &m);
};

struct Wireframe : public Drawable {

	Wireframe(std::string name, std::vector<Coordinate> coordinates) :
		Drawable(name),
		coordinates(coordinates) {}

	std::vector<Coordinate> coordinates;
	std::vector<Coordinate> window_coordinates;

	std::string type();
	Coordinate center();
	void applyMatrix(const Matrix &m);
	void setWindowCoordinates(const Matrix &m);
};

struct Bezier : public Drawable {

	Bezier(std::string name, std::vector<BezierCurve> curves) : 
		Drawable(name),
		curves(curves) {}

	Bezier(std::string name, std::vector<Coordinate> coordinates);
	
	std::vector<BezierCurve> curves;
	std::vector<BezierCurve> window_curves;

	std::string type();
	Coordinate center();
	void applyMatrix(const Matrix &m);
	void setWindowCoordinates(const Matrix &m);
};

struct Spline : public Drawable {

	Spline(std::string name, std::vector<Coordinate> coordinates) : 
		Drawable(name),
		coordinates(coordinates) {}
	
	std::vector<Coordinate> coordinates;
	std::vector<Coordinate> window_coordinates;

	std::string type();
	Coordinate center();
	void applyMatrix(const Matrix &m);
	void setWindowCoordinates(const Matrix &m);
};

#endif