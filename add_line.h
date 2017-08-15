#ifndef ADDLINE_H
#define ADDLINE_H

#include <gtkmm.h>

#include "display_file.h"

class AddLine : public Gtk::Dialog {
    
public:
	AddLine(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
	void setDisplayFile(DisplayFile *displayFile);

protected:
    Glib::RefPtr<Gtk::Builder> builder;
    Gtk::Entry *nameEntry;
    Gtk::Entry *x1Entry;
    Gtk::Entry *y1Entry;
    Gtk::Entry *x2Entry;
    Gtk::Entry *y2Entry;
    Gtk::Button *okButton;
    Gtk::Button *cancelButton;

	void on_ok_button_clicked();
	void on_cancel_button_clicked();

private:
	DisplayFile *displayFile;
};

#endif