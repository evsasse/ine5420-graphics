#include "add_object.h"

#include <string>
#include <iostream>

#include "drawable.h"


AddObject::AddObject(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
Gtk::Dialog(cobject), builder(refGlade) {
	builder->get_widget("NameEntry", pNameEntry);
	builder->get_widget("AddCoordButton", pAddCoordButton);
	builder->get_widget("OkButton", pOkButton);
	builder->get_widget("CancelButton", pCancelButton);
	builder->get_widget("Notebook", pNotebook);
	pCoordinateListStore = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(builder->get_object("CoordinateListStore"));

	pAddCoordButton->signal_clicked().connect(sigc::mem_fun(*this, &AddObject::on_add_coord_button_clicked));
	pOkButton->signal_clicked().connect(sigc::mem_fun(*this, &AddObject::on_ok_button_clicked));
	pCancelButton->signal_clicked().connect(sigc::mem_fun(*this, &AddObject::on_cancel_button_clicked));
	pNotebook->signal_switch_page().connect(sigc::mem_fun(*this, &AddObject::on_switch_notebook_page));
}

int AddObject::run(){
	refresh_list_store_avaiable_rows();

	Gtk::Dialog::run();
}

void AddObject::setDisplayFile(DisplayFile *displayFile) {
	this->pDisplayFile = displayFile;
}

void AddObject::setObjectToEdit(Drawable *drawable) {
	this->pOriginalObject = drawable;
}

void AddObject::on_switch_notebook_page(Gtk::Widget *page, int page_number){
	refresh_list_store_avaiable_rows();
}

void AddObject::on_add_coord_button_clicked(){
	auto row = *(pCoordinateListStore->append());
	row.set_value(XXX, 0);
	row.set_value(YYY, 0);
	refresh_list_store_avaiable_rows();
}

void AddObject::refresh_list_store_avaiable_rows(){
	auto current_tab = pNotebook->get_current_page();
	auto n_avaiable = (current_tab == WIREFRAME) ? 9 : current_tab + 1;

	auto children = pCoordinateListStore->children();
	for(auto iter = children.begin(); iter != children.end(); ++iter)
	{
	  	auto row = *iter;

	  	bool avaiable = n_avaiable-- > 0;

		row.set_value(NOT_AVAIABLE, !avaiable);
		row.set_value(AVAIABLE, avaiable);
	}
}

void AddObject::on_ok_button_clicked(){
	auto current_tab = pNotebook->get_current_page();

	std::string name = pNameEntry->get_buffer()->get_text();

	std::vector<Coordinate> coordinates;

	auto children = pCoordinateListStore->children();
	for(auto iter = children.begin(); iter != children.end(); ++iter)
	{
	  	auto row = *iter;

	  	double x, y;
	  	row.get_value(XXX, x);
	  	row.get_value(YYY, y);

	  	Coordinate coord = Coordinate(x, y);

		coordinates.push_back(coord);
	}

	if(current_tab == POINT){
		pDisplayFile->points.push_back(Point(name, coordinates[0]));
	} else if (current_tab == LINE){
		pDisplayFile->lines.push_back(Line(name, coordinates[0], coordinates[1]));
	} else {
		pDisplayFile->wireframes.push_back(Wireframe(name, coordinates));
	}

	response(0);
}

void AddObject::on_cancel_button_clicked(){
    response(1);
}


