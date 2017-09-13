#include "sgi.h"

#include <iostream>
#include <sstream>

#include "add_object.h"
#include "painter.h"
#include "clipper.h"
#include "viewport.h"

SGI::SGI(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
    Gtk::Window(cobject), builder(refGlade), window(), queueRecalcWindowCoordinates(true)
{
    builder->get_widget("ViewportDrawingArea", pViewportDrawingArea);
    builder->get_widget("StepEntry", pStepEntry);
    builder->get_widget("RotateXEntry", pRotateXEntry);
    builder->get_widget("RotateYEntry", pRotateYEntry);
    builder->get_widget("InButton", pInButton);
    builder->get_widget("OutButton", pOutButton);    
    builder->get_widget("UpButton", pUpButton);
    builder->get_widget("DownButton", pDownButton);
    builder->get_widget("LeftButton", pLeftButton);
    builder->get_widget("RightButton", pRightButton);
    builder->get_widget("AddObjectButton", pAddObjectButton);
    builder->get_widget("SelectWindowButton", pSelectWindowButton);
    builder->get_widget("TurnLeftButton", pTurnLeftButton);
    builder->get_widget("TurnRightButton", pTurnRightButton);
    builder->get_widget("WorldRotateButton", pWorldRotateButton);
    builder->get_widget("ObjectRotateButton", pObjectRotateButton);

    builder->get_widget("ObjectTreeView", pObjectTreeView);
    pObjectListStore = Gtk::ListStore::create(objectColumnRecord);
    pObjectTreeSelection = pObjectTreeView->get_selection();
    pObjectTreeView->set_model(pObjectListStore);
    pObjectTreeView->append_column("Name", objectColumnRecord.col_Name);
    pObjectTreeView->append_column("Type", objectColumnRecord.col_Type);
    
    pViewportDrawingArea->signal_draw().connect(sigc::mem_fun(*this, &SGI::on_viewport_drawing_area_draw));
    pInButton->signal_clicked().connect(sigc::mem_fun(*this, &SGI::on_in_button_clicked));
    pOutButton->signal_clicked().connect(sigc::mem_fun(*this, &SGI::on_out_button_clicked));
    pUpButton->signal_clicked().connect(sigc::mem_fun(*this, &SGI::on_up_button_clicked));
    pDownButton->signal_clicked().connect(sigc::mem_fun(*this, &SGI::on_down_button_clicked));
    pLeftButton->signal_clicked().connect(sigc::mem_fun(*this, &SGI::on_left_button_clicked));
    pRightButton->signal_clicked().connect(sigc::mem_fun(*this, &SGI::on_right_button_clicked));
    pAddObjectButton->signal_clicked().connect(sigc::mem_fun(*this, &SGI::on_add_object_button_clicked));
    pSelectWindowButton->signal_clicked().connect(sigc::mem_fun(*this, &SGI::on_select_window_button_clicked));
    pTurnLeftButton->signal_clicked().connect(sigc::mem_fun(*this, &SGI::on_turn_left_button_clicked));
    pTurnRightButton->signal_clicked().connect(sigc::mem_fun(*this, &SGI::on_turn_right_button_clicked));
    pWorldRotateButton->signal_clicked().connect(sigc::mem_fun(*this, &SGI::on_world_rotate_button_clicked));
    pObjectRotateButton->signal_clicked().connect(sigc::mem_fun(*this, &SGI::on_object_rotate_button_clicked));

    // objetos iniciais 
    Point* p = new Point("point", Coordinate(80, 80));
    displayFile.drawables.push_back(p);

    Line* l = new Line("line", Coordinate(50, 50), Coordinate(150, 100));
    displayFile.drawables.push_back(l);

    std::vector<Coordinate> coordinates;
    coordinates.push_back(Coordinate(100, 100));
    coordinates.push_back(Coordinate(100, 150));
    coordinates.push_back(Coordinate(150, 150));
    coordinates.push_back(Coordinate(150, 100));
    Wireframe* w = new Wireframe("wireframe", coordinates);
    displayFile.drawables.push_back(w);
    
    l = new Line("line2", Coordinate(350, 350), Coordinate(400, 400));
    displayFile.drawables.push_back(l);

    coordinates = std::vector<Coordinate>();
    coordinates.push_back(Coordinate(300, 300));
    coordinates.push_back(Coordinate(320, 300));
    coordinates.push_back(Coordinate(310, 320));
    w = new Wireframe("wireframe2", coordinates);
    displayFile.drawables.push_back(w);

    coordinates = std::vector<Coordinate>();
    coordinates.push_back(Coordinate(200, 200));
    coordinates.push_back(Coordinate(250, 100));
    coordinates.push_back(Coordinate(150, 175));
    coordinates.push_back(Coordinate(300, 175));
    coordinates.push_back(Coordinate(150, 100));
    w = new Wireframe("star", coordinates);
    displayFile.drawables.push_back(w);

    refresh_list_store();
}

void SGI::refresh_list_store()
{
    pObjectListStore->clear();

    for (auto d : displayFile.drawables) {
        add_row(d->name, d->type(), d);
    }
}

void SGI::add_row(std::string name, std::string type, Drawable* object)
{
    Gtk::TreeModel::iterator iter = pObjectListStore->append();
    Gtk::TreeModel::Row row = *iter;
    row[objectColumnRecord.col_Name] = name;
    row[objectColumnRecord.col_Type] = type;
    row[objectColumnRecord.col_Object] = object;
}

Drawable* SGI::get_selected_object()
{
    Gtk::ListStore::iterator iter = pObjectTreeSelection->get_selected();

    if (iter) {
        Gtk::ListStore::Row row = *iter;

        Drawable *object = row[objectColumnRecord.col_Object];

        return object;
    }

    return nullptr;
}

double SGI::get_step_size()
{
    try {
        return std::stod(pStepEntry->get_buffer()->get_text());
    } catch (...) {
        std::cout << "Step size is invalid" << std::endl;
        return 0;
    }
}

Coordinate SGI::get_rotate_coordinate()
{
    double x, y;

    try {
        x = std::stod(pRotateXEntry->get_buffer()->get_text());
    } catch (...) {
        std::cout << "X is invalid" << std::endl;
        x =  0;
    }

    try {
        y = std::stod(pRotateYEntry->get_buffer()->get_text());
    } catch (...) {
        std::cout << "Y is invalid" << std::endl;
        y =  0;
    }

    return Coordinate(x, y);  
}

void SGI::set_rotate_coordinate(Coordinate coordinate)
{
    std::ostringstream strs_x;
    strs_x << coordinate.x;
    std::string str_x = strs_x.str();
    pRotateXEntry->set_text(str_x);

    std::ostringstream strs_y;
    strs_y << coordinate.y;
    std::string str_y = strs_y.str();
    pRotateYEntry->set_text(str_y);
}

bool SGI::on_viewport_drawing_area_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    auto allocation = pViewportDrawingArea->get_allocation();
    // should find a way to better set this up
    // not needing to reset the size at every draw
    // and to the viewport object to be reused
    Viewport viewport(allocation.get_width(), allocation.get_height(), 20);
    window.u_base = viewport.xSize();
    window.v_base = viewport.ySize();

    if(queueRecalcWindowCoordinates){
        queueRecalcWindowCoordinates = false;
        displayFile.update_window_coordinates(Matrix::window_transformation(window));
    }

    Clipper clipper;
    Painter painter(cr, viewport);

    painter.drawBorder();

    for (auto d : displayFile.drawables) {
        painter.draw(clipper.clip(d));
    }

    return true;
}

void SGI::on_add_object_button_clicked()
{
    AddObject *pAddObjectDialog;
    Gtk::Builder::create_from_file("sgi.glade")->get_widget_derived("ObjectFormDialog", pAddObjectDialog);
    pAddObjectDialog->setDisplayFile(&displayFile);

    pAddObjectDialog->run();

    refresh_list_store();

    pViewportDrawingArea->queue_draw();

    delete pAddObjectDialog;
}

void SGI::on_select_window_button_clicked()
{
    pObjectTreeSelection->unselect_all();
}

void SGI::on_up_button_clicked()
{
    Drawable* object = get_selected_object();

    if (object) {
        object->translate(0, 10);
        object->setWindowCoordinates(Matrix::window_transformation(window));
    } else {
        window.y_center += 10;
        queueRecalcWindowCoordinates = true;
    }

    pViewportDrawingArea->queue_draw();
}

void SGI::on_down_button_clicked()
{
    Drawable* object = get_selected_object();

    if (object) {
        object->translate(0, -10);
        object->setWindowCoordinates(Matrix::window_transformation(window));
    } else {
        window.y_center -= 10;
        queueRecalcWindowCoordinates = true;
    }

    pViewportDrawingArea->queue_draw();
}

void SGI::on_left_button_clicked()
{
    Drawable* object = get_selected_object();

    if (object) {
        object->translate(-10, 0);
        object->setWindowCoordinates(Matrix::window_transformation(window));
    } else {
        window.x_center -= 10;
        queueRecalcWindowCoordinates = true;
    }

    pViewportDrawingArea->queue_draw();
}

void SGI::on_right_button_clicked()
{
    Drawable* object = get_selected_object();

    if (object) {
        object->translate(10, 0);
        object->setWindowCoordinates(Matrix::window_transformation(window));
    } else {
        window.x_center += 10;
        queueRecalcWindowCoordinates = true;
    }

    pViewportDrawingArea->queue_draw();
}

void SGI::on_in_button_clicked()
{
    Drawable* object = get_selected_object();

    if (object) {
        object->scale(1.1, 1.1);
        object->setWindowCoordinates(Matrix::window_transformation(window));
    } else {
        window.scale *= 1 + get_step_size() / 100.0;
        queueRecalcWindowCoordinates = true;
    }

    pViewportDrawingArea->queue_draw();
}

void SGI::on_out_button_clicked()
{
    Drawable* object = get_selected_object();

    if (object) {
        object->scale(0.9, 0.9);
        object->setWindowCoordinates(Matrix::window_transformation(window));
    } else {
        window.scale /= 1 + get_step_size() / 100.0;
        queueRecalcWindowCoordinates = true;
    }
    
    pViewportDrawingArea->queue_draw();
}

void SGI::on_turn_left_button_clicked()
{
    Drawable* object = get_selected_object();

    if (object) {
        object->rotate(get_rotate_coordinate(), -10);
        object->setWindowCoordinates(Matrix::window_transformation(window));
    } else {
        window.rotation -= 10;
        queueRecalcWindowCoordinates = true;
    }

    pViewportDrawingArea->queue_draw();
}

void SGI::on_turn_right_button_clicked()
{
    Drawable* object = get_selected_object();

    if (object) {
        object->rotate(get_rotate_coordinate(), 10);
        object->setWindowCoordinates(Matrix::window_transformation(window));
    } else {
        window.rotation += 10;
        queueRecalcWindowCoordinates = true;
    }

    pViewportDrawingArea->queue_draw();
}

void SGI::on_world_rotate_button_clicked()
{
    set_rotate_coordinate(Coordinate(0, 0));
}

void SGI::on_object_rotate_button_clicked()
{
    Drawable* object = get_selected_object();

    if (object) {
        set_rotate_coordinate(object->center());
    }
}