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


struct ObjectColumnRecord : public Gtk::TreeModelColumnRecord
{
    ObjectColumnRecord() {
        add(col_Name);
        add(col_Type);
        add(col_Object);
    }

    Gtk::TreeModelColumn<Glib::ustring> col_Name;
    Gtk::TreeModelColumn<Glib::ustring> col_Type;
    Gtk::TreeModelColumn<Drawable*> col_Object;
};


class SGI : public Gtk::Window
{
public:
    SGI(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);

protected:
    Glib::RefPtr<Gtk::Builder> builder;
	Gtk::DrawingArea* pViewportDrawingArea;
	Gtk::Entry* pStepEntry;
	Gtk::Entry* pRotateXEntry;
	Gtk::Entry* pRotateYEntry;
	Gtk::Button* pAddObjectButton;
	Gtk::Button* pSelectWindowButton;
	Gtk::Button* pUpButton;
	Gtk::Button* pDownButton;
	Gtk::Button* pLeftButton;
	Gtk::Button* pRightButton;
	Gtk::Button* pInButton;
	Gtk::Button* pOutButton;
	Gtk::Button* pTurnLeftButton;
	Gtk::Button* pTurnRightButton;
	Gtk::Button* pWorldRotateButton;
	Gtk::Button* pObjectRotateButton;

	Gtk::TreeView *pObjectTreeView;
	ObjectColumnRecord objectColumnRecord;
    Glib::RefPtr<Gtk::ListStore> pObjectListStore;
    Glib::RefPtr<Gtk::TreeSelection> pObjectTreeSelection;

    void refresh_list_store();
    void add_row(std::string name, std::string type, Drawable* object);

    Drawable* get_selected_object();
    double get_step_size();
    Coordinate get_rotate_coordinate();
    void set_rotate_coordinate(Coordinate coordinate);    

 	bool on_viewport_drawing_area_draw(const Cairo::RefPtr<Cairo::Context>& cr);

	void on_up_button_clicked();
	void on_down_button_clicked();
	void on_left_button_clicked();
	void on_right_button_clicked();
	void on_in_button_clicked();
	void on_out_button_clicked();	
	void on_add_object_button_clicked();
	void on_select_window_button_clicked();
	void on_turn_left_button_clicked();
	void on_turn_right_button_clicked();
	void on_world_rotate_button_clicked();
	void on_window_rotate_button_clicked();
	void on_object_rotate_button_clicked();
	
private:
	DisplayFile displayFile;
	Rectangle window;
	double xVpMax, yVpMax;
};

#endif