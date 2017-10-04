#include "add_object.h"

#include <iostream>

#include "drawable.h"


AddObject::AddObject(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
Gtk::Dialog(cobject), builder(refGlade) {
	builder->get_widget("NameEntry", pNameEntry);
	builder->get_widget("AddCoordButton", pAddCoordButton);
	builder->get_widget("OkButton", pOkButton);
	builder->get_widget("CancelButton", pCancelButton);
	builder->get_widget("Notebook", pNotebook);

	builder->get_widget("PointTreeView", pPointTreeView);
	builder->get_widget("LineTreeView", pLineTreeView);
	builder->get_widget("WireframeTreeView", pWireframeTreeView);
	builder->get_widget("BezierTreeView", pBezierTreeView);
	pCoordinateListStore = Gtk::ListStore::create(coordinateColumnRecord);
	add_columns_to(pPointTreeView);
	add_columns_to(pLineTreeView);
	add_columns_to(pWireframeTreeView);
	add_columns_to(pBezierTreeView);

	pAddCoordButton->signal_clicked().connect(sigc::mem_fun(*this, &AddObject::add_new_coordinate));
	pOkButton->signal_clicked().connect(sigc::mem_fun(*this, &AddObject::on_ok_button_clicked));
	pCancelButton->signal_clicked().connect(sigc::mem_fun(*this, &AddObject::on_cancel_button_clicked));
	pNotebook->signal_switch_page().connect(sigc::mem_fun(*this, &AddObject::on_switch_notebook_page));
}

void AddObject::add_columns_to(Gtk::TreeView *pTreeView){
	pTreeView->set_model(pCoordinateListStore);
	int x_col_pos = pTreeView->append_column_editable("X", coordinateColumnRecord.col_X);
	int y_col_pos = pTreeView->append_column_editable("Y", coordinateColumnRecord.col_Y);

	Gtk::TreeViewColumn* pXColumn = pTreeView->get_column(x_col_pos-1);
	Gtk::CellRendererText* pXColumnRenderer = static_cast<Gtk::CellRendererText*>(pXColumn->get_first_cell());
	pXColumn->add_attribute(pXColumnRenderer->property_editable(), coordinateColumnRecord.col_Available);
	pXColumn->add_attribute(pXColumnRenderer->property_strikethrough(), coordinateColumnRecord.col_NotAvailable);
	// pXColumn->add_attribute(pXColumnRenderer->property_visible(), coordinateColumnRecord.col_NotAvailable);

	Gtk::TreeViewColumn* pYColumn = pTreeView->get_column(y_col_pos-1);
	Gtk::CellRendererText* pYColumnRenderer = static_cast<Gtk::CellRendererText*>(pYColumn->get_first_cell());
	pYColumn->add_attribute(pYColumnRenderer->property_editable(), coordinateColumnRecord.col_Available);
	pYColumn->add_attribute(pYColumnRenderer->property_strikethrough(), coordinateColumnRecord.col_NotAvailable);
}

void AddObject::add_new_coordinate(){
	Gtk::TreeModel::iterator iter = pCoordinateListStore->append();
	Gtk::TreeModel::Row row = *iter;
	row[coordinateColumnRecord.col_X] = 0;
	row[coordinateColumnRecord.col_Y] = 0;

	refresh_list_store_avaiable_rows();
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

void AddObject::refresh_list_store_avaiable_rows(){
	auto current_tab = pNotebook->get_current_page();

	int n_avaiable = 0;
	int rows = pCoordinateListStore->children().size();

	switch(current_tab){
		case POINT: n_avaiable = 1; break;
		case LINE: n_avaiable = 2; break;
		case WIREFRAME: n_avaiable = rows; break;
		default: n_avaiable = ((rows-1) / 3) * 3  + 1;
	}

	auto children = pCoordinateListStore->children();
	for(auto iter = children.begin(); iter != children.end(); ++iter)
	{
	  	auto row = *iter;

	  	bool avaiable = n_avaiable-- > 0;

	  	row[coordinateColumnRecord.col_Available] = avaiable;
		row[coordinateColumnRecord.col_NotAvailable] = !avaiable;
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

	  	double x = row[coordinateColumnRecord.col_X];
	  	double y = row[coordinateColumnRecord.col_Y];

	  	Coordinate coord = Coordinate(x, y);

		coordinates.push_back(coord);
	}

	if(current_tab == POINT){
		if(coordinates.size() < 1){
			return;
		}
		pDisplayFile->drawables.push_back(new Point(name, coordinates[0]));
	}
	else if (current_tab == LINE){
		if(coordinates.size() < 2){
			return;
		}
		pDisplayFile->drawables.push_back(new Line(name, coordinates[0], coordinates[1]));
	}
	else if(current_tab == WIREFRAME){
		if(coordinates.size() < 3){
			return;
		}

		pDisplayFile->drawables.push_back(new Wireframe(name, coordinates));
	}
	else if(current_tab == BEZIER){
		if(coordinates.size() < 4){
			return;
		}

		pDisplayFile->drawables.push_back(new Bezier(name, coordinates));
	}

	response(0);
}

void AddObject::on_cancel_button_clicked(){
    response(1);
}
