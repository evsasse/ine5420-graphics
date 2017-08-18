#ifndef ADDOBJECT_H
#define ADDOBJECT_H

#include <gtkmm.h>

#include "display_file.h"

class AddObject : public Gtk::Dialog {
public:
	AddObject(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
    void setDisplayFile(DisplayFile *displayFile);
	void setObjectToEdit(Drawable *drawable);
    int run();

protected:
    Glib::RefPtr<Gtk::Builder> builder;
    Gtk::Entry *pNameEntry;
    Gtk::Button *pAddCoordButton;
    Gtk::Button *pCancelButton;
    Gtk::Button *pOkButton;
    Gtk::Frame *pCForm1;
    Gtk::Frame *pCForm2;
    Gtk::Notebook *pNotebook;

    Gtk::ListBox *pPointListBox;

    Glib::RefPtr<Gtk::ListStore> pCoordinateListStore;
    Gtk::TreeModelColumn<double> xColumn;
    Gtk::TreeModelColumn<double> yColumn;
    Gtk::TreeModelColumn<bool> avaiableColumn;
    Gtk::TreeModelColumn<bool> notAvaiableColumn;

    void refresh_list_store_avaiable_rows();

    void on_ok_button_clicked();
	void on_add_coord_button_clicked();
    void on_switch_notebook_page(Gtk::Widget* page, int page_number);
	void on_cancel_button_clicked();

private:
	DisplayFile *pDisplayFile;
    Drawable *pOriginalObject;

    enum NotebookTab { POINT=0, LINE=1, WIREFRAME=2 };
    enum ListStoreField { XXX=0, YYY=1, NOT_AVAIABLE=2, AVAIABLE=3 };
};

#endif