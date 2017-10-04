#include <iostream>

#include "drawable.h"

Coordinate Coordinate::applyMatrix(const Matrix &m) const
{
	double new_x = x * m.v[0][0] + y * m.v[1][0] + m.v[2][0];
    double new_y = x * m.v[0][1] + y * m.v[1][1] + m.v[2][1];

    return Coordinate(new_x, new_y);
}



BezierCurve BezierCurve::applyMatrix(const Matrix &m) const
{
	Coordinate new_c1 = c1.applyMatrix(m);
	Coordinate new_c2 = c2.applyMatrix(m);
	Coordinate new_c3 = c3.applyMatrix(m);
	Coordinate new_c4 = c4.applyMatrix(m);

    return BezierCurve(new_c1, new_c2, new_c3, new_c4);
}



void Drawable::translate(double dx, double dy)
{
	applyMatrix(Matrix::translation(dx, dy));
}

void Drawable::scale(double sx, double sy)
{
	Coordinate c = center();
	applyMatrix(Matrix::scaling(c.x, c.y, sx, sy));
}

void Drawable::rotate(double deg)
{
	rotate(center(), deg);
}

void Drawable::rotate(const Coordinate &c, double deg)
{
	applyMatrix(Matrix::rotation(c.x, c.y, deg));
}



std::string Point::type()
{
	return "Point";
}

Coordinate Point::center()
{
	return coordinate;
}

void Point::applyMatrix(const Matrix &m)
{
	coordinate = coordinate.applyMatrix(m);
}

void Point::setWindowCoordinates(const Matrix &m)
{
	window_coordinate = coordinate.applyMatrix(m);
}



std::string Line::type()
{
	return "Line";
}

Coordinate Line::center()
{
	double cx = (coordinate_a.x + coordinate_b.x) / 2;
    double cy = (coordinate_a.y + coordinate_b.y) / 2;

    return Coordinate(cx, cy);
}

void Line::applyMatrix(const Matrix &m)
{
	coordinate_a = coordinate_a.applyMatrix(m);
    coordinate_b = coordinate_b.applyMatrix(m);
}

void Line::setWindowCoordinates(const Matrix &m)
{
	window_coordinate_a = coordinate_a.applyMatrix(m);
    window_coordinate_b = coordinate_b.applyMatrix(m);
}



std::string Wireframe::type()
{
	return "Wireframe";
}

Coordinate Wireframe::center()
{
    double cx = 0;
    double cy = 0;

    for (auto coordinate : coordinates) {
        cx += coordinate.x;
        cy += coordinate.y;
    }

    cx /= coordinates.size();
    cy /= coordinates.size();

    return Coordinate(cx, cy);
}

void Wireframe::applyMatrix(const Matrix &m)
{
	std::vector<Coordinate> new_coordinates;

	for (auto coordinate : coordinates) {
		new_coordinates.push_back(coordinate.applyMatrix(m));
	}

    coordinates = new_coordinates;
}

void Wireframe::setWindowCoordinates(const Matrix &m)
{
	std::vector<Coordinate> new_coordinates;

	for (auto coordinate : coordinates) {
		new_coordinates.push_back(coordinate.applyMatrix(m));
	}

    window_coordinates = new_coordinates;
}


Bezier::Bezier(std::string name, std::vector<Coordinate> coordinates) :
	Drawable(name)
{
	int max = 4;

	while(max <= coordinates.size()){
		curves.push_back(BezierCurve(coordinates[max-3],
			                         coordinates[max-2],
			                         coordinates[max-1],
			                         coordinates[max]));

		max += 3;
	}
}

std::string Bezier::type()
{
	return "Bezier";
}

Coordinate Bezier::center()
{
    double cx = (curves[0].c1.x + curves[curves.size() - 1].c4.x) / 2;
    double cy = (curves[0].c1.y + curves[curves.size() - 1].c4.y) / 2;

    return Coordinate(cx, cy);
}

void Bezier::applyMatrix(const Matrix &m)
{
	std::vector<BezierCurve> new_curves;

	for (auto curve : curves) {
		new_curves.push_back(curve.applyMatrix(m));
	}

    curves = new_curves;
}

void Bezier::setWindowCoordinates(const Matrix &m)
{
	std::vector<BezierCurve> new_curves;

	for (auto curve : curves) {
		new_curves.push_back(curve.applyMatrix(m));
	}

    window_curves = new_curves;
}


std::string Spline::type()
{
	return "Spline";
}

Coordinate Spline::center()
{
    double cx = 0;
    double cy = 0;

    for (auto coordinate : coordinates) {
        cx += coordinate.x;
        cy += coordinate.y;
    }

    cx /= coordinates.size();
    cy /= coordinates.size();

    return Coordinate(cx, cy);
}

void Spline::applyMatrix(const Matrix &m)
{
	std::vector<Coordinate> new_coordinates;

	for (auto coordinate : coordinates) {
		new_coordinates.push_back(coordinate.applyMatrix(m));
	}

    coordinates = new_coordinates;
}

void Spline::setWindowCoordinates(const Matrix &m)
{
	std::vector<Coordinate> new_coordinates;

	for (auto coordinate : coordinates) {
		new_coordinates.push_back(coordinate.applyMatrix(m));
	}

    window_coordinates = new_coordinates;
}