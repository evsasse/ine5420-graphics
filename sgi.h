#ifndef SGI_H
#define SGI_H

#include <gtkmm.h>

#include <cairomm/context.h>

#include <vector>
#include <string>
#include <math.h>

#include "drawable.h"

#include "display_file.h"

#include "structs.h"

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

	Gtk::TreeView *pObjectTreeView;
	ObjectColumnRecord objectColumnRecord;
    Glib::RefPtr<Gtk::ListStore> pObjectListStore;

    void refresh_list_store();
    void add_row(std::string name, std::string type);

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

    Coordinate centerOfLine(const Line &l);
    Coordinate centerOfWireframe(const Wireframe &w);

    Coordinate applyMatrixOnCoordinate(const Coordinate &c, const Matrix &m);
    Line applyMatrixOnLine(const Line &l, const Matrix &m);
    Wireframe applyMatrixOnWireframe(const Wireframe &w, const Matrix &m);

    Point translatePoint(const Point &p, double dx, double dy);
    Line translateLine(const Line &l, double dx, double dy);
    Wireframe translateWireframe(const Wireframe &w, double dx, double dy);

    Line scaleLine(const Line &l, double sx, double sy);
    Wireframe scaleWireframe(const Wireframe &w, double sx, double sy);

    Line rotateLine(const Line &l, double degrees);
    Line rotateLine(const Line &l, const Coordinate &c, double degrees);
    Wireframe rotateWireframe(const Wireframe &w, double degrees);
    Wireframe rotateWireframe(const Wireframe &w, const Coordinate &c, double degrees);

    

private:
	DisplayFile displayFile;
	Rectangle window;
	double xVpMax, yVpMax;
};

#endif