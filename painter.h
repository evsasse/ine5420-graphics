#ifndef PAINTER_H
#define PAINTER_H

#include <cairomm/context.h>

#include "drawable.h"
#include "viewport.h"

#include <math.h>

class Painter
{
public:
	Painter(const Cairo::RefPtr<Cairo::Context>& cr, const Viewport& vp) :
		cr(cr), vp(vp) {}


	void draw(Drawable *pDrawable)
	{
		if (!pDrawable) {
			// in case of the clipper not returning a new drawable
			return;
		}

		Point *point = dynamic_cast<Point *>(pDrawable);
		if (point) {
			return draw(point);
		}

		Line *line = dynamic_cast<Line *>(pDrawable);
		if (line) {
			return draw(line);
		}

		Wireframe *wireframe = dynamic_cast<Wireframe *>(pDrawable);
		if (wireframe) {
			return draw(wireframe);
		}

		Bezier *bezier = dynamic_cast<Bezier *>(pDrawable);
		if (bezier) {
			return draw(bezier);
		}

		Spline *spline = dynamic_cast<Spline *>(pDrawable);
		if (spline) {
			return draw(spline);
		}

		return;
	}

	void drawBorder(){
		cr->set_source_rgb(1, 0, 0);
		cr->move_to(vp.xMin, vp.yMin);
	    cr->line_to(vp.xMax, vp.yMin);
	    cr->line_to(vp.xMax, vp.yMax);
	    cr->line_to(vp.xMin, vp.yMax);
	    cr->close_path();
	    cr->stroke();
	}

protected:
	const Cairo::RefPtr<Cairo::Context>& cr;
	const Viewport& vp;

	Coordinate mapToViewport(const Coordinate &c)
	{
		// double new_x = (c.x  + 1) * xVpMax / (2.0);
		// double new_y = (1 - (c.y + 1)  / 2.0) * yVpMax;
		// double new_x = vp.xMin + (vp.xMax / 2) + (c.x * (vp.xMax / 2))

		// remembering: on the viewport the coordinate system starts at the top left
		// thats why these transformatiosn may seem strange at first glance
		double new_x = vp.xMin + ((1 + c.x) / 2.0) * vp.xSize();
		double new_y = vp.yMin + (1 - (1 + c.y) / 2.0) * vp.ySize();

		return Coordinate(new_x, new_y);
	}

	void draw(Point *pPoint)
	{
		cr->set_source_rgb(0, 0, 0);

		auto vpCoordinate = mapToViewport(pPoint->window_coordinate);

	    double x = vpCoordinate.x;
	    double y = vpCoordinate.y;

	    cr->move_to(x-2, y-2);
	    cr->line_to(x+2, y-2);
	    cr->line_to(x+2, y+2);
	    cr->line_to(x-2, y+2);
	    cr->fill();
	}

	void draw(Line *pLine)
	{
		cr->set_source_rgb(0, 0, 0);

		auto vpCoordinate_a = mapToViewport(pLine->window_coordinate_a);
	    auto vpCoordinate_b = mapToViewport(pLine->window_coordinate_b);

	    cr->move_to(vpCoordinate_a.x, vpCoordinate_a.y);
	    cr->line_to(vpCoordinate_b.x, vpCoordinate_b.y);
	    cr->stroke();
	}

	void draw(Wireframe *pWireframe)
	{
		cr->set_source_rgb(0, 0, 0);
		
		auto firstVpCoordinate = mapToViewport(pWireframe->window_coordinates[0]);

	    cr->move_to(firstVpCoordinate.x, firstVpCoordinate.y);

	    for (int i = 1; i < pWireframe->window_coordinates.size(); ++i) {
	        auto vpCoordinate = mapToViewport(pWireframe->window_coordinates[i]);
	        cr->line_to(vpCoordinate.x, vpCoordinate.y);
	    }

	    cr->line_to(firstVpCoordinate.x, firstVpCoordinate.y);
	    cr->stroke();
	}

	void draw(Bezier *pBezier)
	{
		cr->set_source_rgb(0, 0, 0);

		auto firstVpCoordinate = mapToViewport(pBezier->window_curves[0].c1);

		cr->move_to(firstVpCoordinate.x, firstVpCoordinate.y);

		for (int i = 0; i < pBezier->window_curves.size(); ++i) {
			for (size_t t = 0; t <= 100; t++) {
				auto vpCoordinate = mapToViewport(bezier_blending(t/100.0, pBezier->window_curves[i]));
				cr->line_to(vpCoordinate.x, vpCoordinate.y);
			}
		}

		cr->stroke();
	}

	Coordinate bezier_blending(double t, const BezierCurve &curve) {
		double x, y;

		double c1_factor = - pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1;
		double c2_factor = 3 * pow(t, 3) - 6 * pow(t, 2) + 3 * t;
		double c3_factor = - 3 * pow(t, 3) + 3 * pow(t, 2);
		double c4_factor = pow(t, 3);

		x = curve.c1.x * c1_factor + curve.c2.x * c2_factor + curve.c3.x * c3_factor + curve.c4.x * c4_factor;
		y = curve.c1.y * c1_factor + curve.c2.y * c2_factor + curve.c3.y * c3_factor + curve.c4.y * c4_factor;

		return Coordinate(x, y);
	}

	void draw(Spline *pSpline)
	{
		cr->set_source_rgb(0, 1, 0);
		
		auto firstVpCoordinate = mapToViewport(pSpline->window_coordinates[0]);

	    cr->move_to(firstVpCoordinate.x, firstVpCoordinate.y);

	    for (int i = 1; i < pSpline->window_coordinates.size(); ++i) {
	        auto vpCoordinate = mapToViewport(pSpline->window_coordinates[i]);
	        cr->line_to(vpCoordinate.x, vpCoordinate.y);
	    }

	    cr->stroke();
	}
};


#endif // PAINTER_H_