#ifndef PAINTER_H
#define PAINTER_H

#include <cairomm/context.h>

#include "drawable.h"
#include "viewport.h"

class Painter
{
public:
	Painter(const Cairo::RefPtr<Cairo::Context>& cr, const Viewport& vp) :
		cr(cr), vp(vp) {}


	void draw(Drawable *pDrawable)
	{
		if(!pDrawable){
			// in case of the clipper not returning a new drawable
			return;
		}

		Point *point = dynamic_cast<Point *>(pDrawable);
		if(point){
			return draw(point);
		}

		Line *line = dynamic_cast<Line *>(pDrawable);
		if(line){
			return draw(line);
		}

		return draw((Wireframe *) pDrawable);
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

};


#endif // PAINTER_H_