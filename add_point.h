#ifndef ADDPOINT_H
#define ADDPOINT_H

#include <gtkmm.h>

#include "display_file.h"

class AddPoint : public Gtk::Dialog {
    
public:
	AddPoint(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
	void setDisplayFile(DisplayFile *displayFile);

protected:
    Glib::RefPtr<Gtk::Builder> builder;
    Gtk::Entry *nameEntry;
    Gtk::Entry *xEntry;
    Gtk::Entry *yEntry;
    Gtk::Button *okButton;
    Gtk::Button *cancelButton;

	void on_ok_button_clicked();
	void on_cancel_button_clicked();

private:
	DisplayFile *displayFile;
};

#endif