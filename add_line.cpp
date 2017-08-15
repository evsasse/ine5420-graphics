#include "add_line.h"

#include <string>

#include "drawable.h"


AddLine::AddLine(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
Gtk::Dialog(cobject), builder(refGlade) {
	builder->get_widget("nameEntry", nameEntry);
	builder->get_widget("x1Entry", x1Entry);
	builder->get_widget("y1Entry", y1Entry);
	builder->get_widget("x2Entry", x2Entry);
	builder->get_widget("y2Entry", y2Entry);
	builder->get_widget("okButton", okButton);
	builder->get_widget("cancelButton", cancelButton);

	okButton->signal_clicked().connect(sigc::mem_fun(*this, &AddLine::on_ok_button_clicked));
	cancelButton->signal_clicked().connect(sigc::mem_fun(*this, &AddLine::on_cancel_button_clicked));

}

void AddLine::setDisplayFile(DisplayFile *displayFile) {
	this->displayFile = displayFile;
}

void AddLine::on_ok_button_clicked(){
	std::string name = nameEntry->get_buffer()->get_text();
	double x1 = std::stod(x1Entry->get_buffer()->get_text());
	double y1 = std::stod(y1Entry->get_buffer()->get_text());
	double x2 = std::stod(x2Entry->get_buffer()->get_text());
	double y2 = std::stod(y2Entry->get_buffer()->get_text());

	displayFile->lines.push_back(Line(name, Coordinate(x1, y1), Coordinate(x2, y2)));

	response(0);
}

void AddLine::on_cancel_button_clicked(){
    response(1);
}


