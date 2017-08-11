#ifndef DRAWABLE
#define DRAWABLE

#include <vector>

using namespace std;

class Coordinate {
public:
	Coordinate(double x, double y) :
	x(x), y(y)
	{};

	double x, y;
};

class Drawable{
public:
	virtual ~Drawable() {};
	// virtual void draw(const Cairo::RefPtr<Cairo::Context>& cr) const;
};

class Point : public Drawable {
public:
	Point(Coordinate coordinate) :
		coordinate(coordinate) {};

	Coordinate coordinate;
};

class Line : public Drawable {
public:
	Line(Coordinate coordinate_a, Coordinate coordinate_b) :
		coordinate_a(coordinate_a), coordinate_b(coordinate_b)
		{};

	Coordinate coordinate_a, coordinate_b;
};

class Polygon : public Drawable {
public:
	Polygon(vector<Coordinate> coordinates) :
		coordinates(coordinates)
		{};

	vector<Coordinate> coordinates;

	// void draw(const Cairo::RefPtr<Cairo::Context>& cr) const{

	// }
};

#endif