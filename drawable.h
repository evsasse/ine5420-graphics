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

	Drawable(std::string nome) : 
		nome(nome) {}

	std::string nome;
};

struct Point : public Drawable {

	Point(std::string nome, Coordinate coordinate) :
		Drawable(nome), coordinate(coordinate) {}

	Coordinate coordinate;
};

struct Line : public Drawable {

	Line(std::string nome, Coordinate coordinate_a, Coordinate coordinate_b) :
		Drawable(nome), coordinate_a(coordinate_a), coordinate_b(coordinate_b)
		{}

	Coordinate coordinate_a, coordinate_b;
};

struct Wireframe : public Drawable {

	Wireframe(std::string nome, std::vector<Coordinate> coordinates) :
		Drawable(nome), coordinates(coordinates)
		{}

	std::vector<Coordinate> coordinates;

};

#endif