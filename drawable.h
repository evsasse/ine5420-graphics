#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <vector>
#include <string>

#include "structs.h"
#include <cairomm/context.h>

struct Coordinate {

	Coordinate(double x, double y) :
		x(x), y(y) {}

	// world coordinates
	double x, y;

	// window coordinates, are calculated as needed.
	double u, v;

	Coordinate applyMatrix(const Matrix &m) const;
	Coordinate mapToViewport(const Rectangle window, double xVpMax, double yVpMax);
};

struct Drawable {

	Drawable(std::string name) : 
		name(name) {}

	std::string name;

	virtual std::string type() = 0;
	virtual void draw(const Cairo::RefPtr<Cairo::Context>& cr, const Rectangle window, double xVpMax, double yVpMax) = 0;
	virtual Coordinate center() = 0;
	virtual void applyMatrix(const Matrix &m) = 0;
	void translate(double dx, double dy);
	void scale(double sx, double sy);
	void rotate(double deg);
	void rotate(Coordinate c, double deg);
};

struct Point : public Drawable {

	Point(std::string name, Coordinate coordinate) :
		Drawable(name), coordinate(coordinate) {}

	Coordinate coordinate;

	std::string type();
	void draw(const Cairo::RefPtr<Cairo::Context>& cr, const Rectangle window, double xVpMax, double yVpMax);
	Coordinate center();
	void applyMatrix(const Matrix &m);
};

struct Line : public Drawable {

	Line(std::string name, Coordinate coordinate_a, Coordinate coordinate_b) :
		Drawable(name), coordinate_a(coordinate_a), coordinate_b(coordinate_b)
		{}

	Coordinate coordinate_a, coordinate_b;

	std::string type();
	void draw(const Cairo::RefPtr<Cairo::Context>& cr, const Rectangle window, double xVpMax, double yVpMax);
	Coordinate center();
	void applyMatrix(const Matrix &m);
};

struct Wireframe : public Drawable {

	Wireframe(std::string name, std::vector<Coordinate> coordinates) :
		Drawable(name), coordinates(coordinates)
		{}

	std::vector<Coordinate> coordinates;

	std::string type();
	void draw(const Cairo::RefPtr<Cairo::Context>& cr, const Rectangle window, double xVpMax, double yVpMax);
	Coordinate center();
	void applyMatrix(const Matrix &m);
};

#endif