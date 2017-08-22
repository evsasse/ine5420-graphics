#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <vector>
#include <string>

struct Coordinate {

	Coordinate(double x, double y) :
		x(x), y(y) {}

	double x, y;
};

struct Drawable {

	Drawable(std::string name) : 
		name(name) {}

	std::string name;
};

struct Point : public Drawable {

	Point(std::string name, Coordinate coordinate) :
		Drawable(name), coordinate(coordinate) {}

	Coordinate coordinate;
};

struct Line : public Drawable {

	Line(std::string name, Coordinate coordinate_a, Coordinate coordinate_b) :
		Drawable(name), coordinate_a(coordinate_a), coordinate_b(coordinate_b)
		{}

	Coordinate coordinate_a, coordinate_b;
};

struct Wireframe : public Drawable {

	Wireframe(std::string name, std::vector<Coordinate> coordinates) :
		Drawable(name), coordinates(coordinates)
		{}

	std::vector<Coordinate> coordinates;

};

#endif