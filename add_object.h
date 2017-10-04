#ifndef ADDOBJECT_H
#define ADDOBJECT_H

#include <string>

#include <gtkmm.h>

#include "display_file.h"

class CoordinateColumnRecord : public Gtk::TreeModelColumnRecord
{
public:

    CoordinateColumnRecord() {
        add(col_X);
        add(col_Y);
        add(col_Available);
        add(col_NotAvailable);
    }

    Gtk::TreeModelColumn<double> col_X;
    Gtk::TreeModelColumn<double> col_Y;
    Gtk::TreeModelColumn<bool> col_Available;
    Gtk::TreeModelColumn<bool> col_NotAvailable;
};

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
    Gtk::Notebook *pNotebook;

    Gtk::TreeView *pPointTreeView;
    Gtk::TreeView *pLineTreeView;
    Gtk::TreeView *pWireframeTreeView;
    Gtk::TreeView *pBezierTreeView;
    Gtk::TreeView *pSplineTreeView;
    CoordinateColumnRecord coordinateColumnRecord;
    Glib::RefPtr<Gtk::ListStore> pCoordinateListStore;

    void add_columns_to(Gtk::TreeView *pTreeView);
    void refresh_list_store_avaiable_rows();
    void add_new_coordinate();

    void on_ok_button_clicked();
    void on_switch_notebook_page(Gtk::Widget* page, int page_number);
    void on_cancel_button_clicked();

private:
    DisplayFile *pDisplayFile;
    Drawable *pOriginalObject;

    enum NotebookTab { POINT=0, LINE=1, WIREFRAME=2, BEZIER=3, SPLINE=4 };
};

#endif