#include <iostream>

#include "drawable.h"

Coordinate Coordinate::applyMatrix(const Matrix &m) const
{
	double new_x = x * m.v[0][0] + y * m.v[1][0] + m.v[2][0];
    double new_y = x * m.v[0][1] + y * m.v[1][1] + m.v[2][1];

    return Coordinate(new_x, new_y);
}

Coordinate Coordinate::mapToViewport(const Rectangle window, double xVpMax, double yVpMax)
{
    double new_x = (x - window.xMin) * xVpMax / (window.xMax - window.xMin);
    double new_y = (1 - (y - window.yMin)  / (window.yMax - window.yMin)) * yVpMax;

    return Coordinate(new_x, new_y);    
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

void Drawable::rotate(Coordinate c, double deg)
{
	applyMatrix(Matrix::rotation(c.x, c.y, deg));
}



std::string Point::type()
{
	return "Point";
}

void Point::draw(const Cairo::RefPtr<Cairo::Context>& cr, const Rectangle window, double xVpMax, double yVpMax)
{
	auto vpCoordinate = coordinate.mapToViewport(window, xVpMax, yVpMax);

    double x = vpCoordinate.x;
    double y = vpCoordinate.y;

    cr->move_to(x-2, y-2);
    cr->line_to(x+2, y-2);
    cr->line_to(x+2, y+2);
    cr->line_to(x-2, y+2);
    cr->fill();
}

Coordinate Point::center()
{
	return coordinate;
}

void Point::applyMatrix(const Matrix &m)
{
	coordinate = coordinate.applyMatrix(m);
}



std::string Line::type()
{
	return "Line";
}

void Line::draw(const Cairo::RefPtr<Cairo::Context>& cr, const Rectangle window, double xVpMax, double yVpMax)
{
	auto vpCoordinate_a = coordinate_a.mapToViewport(window, xVpMax, yVpMax);
    auto vpCoordinate_b = coordinate_b.mapToViewport(window, xVpMax, yVpMax);

    cr->move_to(vpCoordinate_a.x, vpCoordinate_a.y);
    cr->line_to(vpCoordinate_b.x, vpCoordinate_b.y);
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



std::string Wireframe::type()
{
	return "Wireframe";
}

void Wireframe::draw(const Cairo::RefPtr<Cairo::Context>& cr, const Rectangle window, double xVpMax, double yVpMax)
{
	auto firstVpCoordinate = coordinates[0].mapToViewport(window, xVpMax, yVpMax);

    cr->move_to(firstVpCoordinate.x, firstVpCoordinate.y);

    for (int i = 1; i < coordinates.size(); ++i) {
        auto vpCoordinate = coordinates[i].mapToViewport(window, xVpMax, yVpMax);
        cr->line_to(vpCoordinate.x, vpCoordinate.y);
    }

    cr->line_to(firstVpCoordinate.x, firstVpCoordinate.y);
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