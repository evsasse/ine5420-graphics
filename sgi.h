#ifndef SGI_H
#define SGI_H

#include <gtkmm.h>

#include <cairomm/context.h>

#include <vector>
#include <string>

#include "drawable.h"

#include "display_file.h"

class ObjectColumnRecord : public Gtk::TreeModelColumnRecord
{
public:

    ObjectColumnRecord() {
        add(col_Name);
        add(col_Type);
    }

    Gtk::TreeModelColumn<Glib::ustring> col_Name;
    Gtk::TreeModelColumn<Glib::ustring> col_Type;
};

struct Rectangle
{
	Rectangle() : 
		xMin(0), yMin(0), xMax(0), yMax(0) {}

	Rectangle(double xMin, double yMin, double xMax, double yMax) : 
		xMin(xMin), yMin(yMin), xMax(xMax), yMax(yMax) {}

	double xMin, yMin, xMax, yMax; 		
};

struct Matrix {
	Matrix() {
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				v[i][j] = 0;
			}
		} 
	}

	double v[3][3];
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
    Coordinate applyMatrixOnCoordinate(const Coordinate &c, const Matrix &m);
    Point applyMatrixOnPoint(const Point &p, const Matrix &m);
    Line applyMatrixOnLine(const Line &l, const Matrix &m);
    Wireframe applyMatrixOnWireframe(const Wireframe &w, const Matrix &m);


private:
	DisplayFile displayFile;
	Rectangle window;
	double xVpMax, yVpMax;
};

#endif