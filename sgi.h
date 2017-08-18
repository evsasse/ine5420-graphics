#ifndef SGI_H
#define SGI_H

#include <gtkmm.h>

#include <cairomm/context.h>

#include <vector>

#include "drawable.h"

#include "display_file.h"

struct Rectangle
{
	double xMin, yMin, xMax, yMax; 

	Rectangle() : 
		xMin(0), yMin(0), xMax(0), yMax(0) {}

	Rectangle(double xMin, double yMin, double xMax, double yMax) : 
		xMin(xMin), yMin(yMin), xMax(xMax), yMax(yMax) {}
};


class SGI : public Gtk::Window {

public:
    SGI(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);

protected:
    Glib::RefPtr<Gtk::Builder> builder;
	Gtk::DrawingArea* pViewportDrawingArea;
	Gtk::Entry* pStepEntry;
	Gtk::Button* pAddObjectButton;
	Gtk::Button* pUpButton;
	Gtk::Button* pDownButton;
	Gtk::Button* pLeftButton;
	Gtk::Button* pRightButton;
	Gtk::Button* pInButton;
	Gtk::Button* pOutButton;		

	void on_up_button_clicked();
	void on_down_button_clicked();
	void on_left_button_clicked();
	void on_right_button_clicked();
	void on_in_button_clicked();
	void on_out_button_clicked();
	bool on_viewport_drawing_area_draw(const Cairo::RefPtr<Cairo::Context>& cr);
	void on_add_object_button_clicked();

    void draw_point(const Cairo::RefPtr<Cairo::Context>& cr, const Point &p);
	void draw_line(const Cairo::RefPtr<Cairo::Context>& cr, const Line &l);
	void draw_wireframe(const Cairo::RefPtr<Cairo::Context>& cr, const Wireframe &w);

    Coordinate mapToViewport(const Coordinate &c);

private:
	DisplayFile displayFile;
	Rectangle window;
	double xVpMax, yVpMax;
};

#endif