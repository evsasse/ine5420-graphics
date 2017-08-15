#include "add_point.h"

#include <string>

#include "drawable.h"


AddPoint::AddPoint(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
Gtk::Dialog(cobject), builder(refGlade) {
	builder->get_widget("nameEntry", nameEntry);
	builder->get_widget("xEntry", xEntry);
	builder->get_widget("yEntry", yEntry);
	builder->get_widget("okButton", okButton);
	builder->get_widget("cancelButton", cancelButton);

	okButton->signal_clicked().connect(sigc::mem_fun(*this, &AddPoint::on_ok_button_clicked));
	cancelButton->signal_clicked().connect(sigc::mem_fun(*this, &AddPoint::on_cancel_button_clicked));

}

void AddPoint::setDisplayFile(DisplayFile *displayFile) {
	this->displayFile = displayFile;
}

void AddPoint::on_ok_button_clicked(){
	std::string name = nameEntry->get_buffer()->get_text();
	double x = std::stod(xEntry->get_buffer()->get_text());
	double y = std::stod(yEntry->get_buffer()->get_text());

	displayFile->points.push_back(Point(name, Coordinate(x, y)));

	response(0);
}

void AddPoint::on_cancel_button_clicked(){
    response(1);
}


