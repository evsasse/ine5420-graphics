#include "sgi.h"

#include <iostream>

#include "add_object.h"

SGI::SGI(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
    Gtk::Window(cobject), builder(refGlade) {

    // auto refGlade = refGlade;
    
    builder->get_widget("ViewportDrawingArea", pViewportDrawingArea);
    builder->get_widget("StepEntry", pStepEntry);
    builder->get_widget("InButton", pInButton);
    builder->get_widget("OutButton", pOutButton);    
    builder->get_widget("UpButton", pUpButton);
    builder->get_widget("DownButton", pDownButton);
    builder->get_widget("LeftButton", pLeftButton);
    builder->get_widget("RightButton", pRightButton);
    builder->get_widget("AddObjectButton", pAddObjectButton);

    builder->get_widget("ObjectTreeView", pObjectTreeView);
    pObjectListStore = Gtk::ListStore::create(objectColumnRecord);
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

    // no construtor, valores dessas funções estão retornando 1
    // xVpMax = pViewportDrawingArea->get_allocation().get_width();
    // yVpMax = pViewportDrawingArea->get_allocation().get_height();
    // por enquanto window inicia hardcoded

    xVpMax = 408;
    yVpMax = 447;

    window.xMax = 408;
    window.yMax = 447;

    // objetos iniciais 

    displayFile.points.push_back(translatePoint(Point("point", Coordinate(80, 80)), 0, 0));
    displayFile.points.push_back(translatePoint(Point("point", Coordinate(-30, -30)), 0, 0));

    //displayFile.lines.push_back(Line("line", Coordinate(170, 170), Coordinate(220, 270)));

    Line l("asd", Coordinate(0, 0), Coordinate(50, 50));
    displayFile.lines.push_back(scaleLine(l, 2, 2));

    displayFile.lines.push_back(rotateLine(l, 90));

    std::vector<Coordinate> coordinates;
    coordinates.push_back(Coordinate(100, 100));
    coordinates.push_back(Coordinate(100, 150));
    coordinates.push_back(Coordinate(150, 150));
    coordinates.push_back(Coordinate(150, 100));

    Wireframe w("wireframe", coordinates);

    displayFile.wireframes.push_back(w);

    // rotacao no centro do mundo
    displayFile.wireframes.push_back(rotateWireframe(w, Coordinate(0, 0), 30));

    // rotacao no centro do objeto
    displayFile.wireframes.push_back(scaleWireframe(rotateWireframe(w, 30), 2, 2));

    // rotacao em ponto qualquer
    displayFile.wireframes.push_back(rotateWireframe(w, Coordinate(200, 200), 30));

    refresh_list_store();
}

void SGI::refresh_list_store()
{
    pObjectListStore->clear();

    for (auto p : displayFile.points){
        add_row(p.name, "Point");
    }
    for (auto p : displayFile.lines){
        add_row(p.name, "Line");
    }
    for (auto p : displayFile.wireframes){
        add_row(p.name, "Wireframe");
    }
}

void SGI::add_row(std::string name, std::string type)
{
    Gtk::TreeModel::iterator iter = pObjectListStore->append();
    Gtk::TreeModel::Row row = *iter;
    row[objectColumnRecord.col_Name] = name;
    row[objectColumnRecord.col_Type] = type;
}
void SGI::on_up_button_clicked()
{
    window.yMin += 10;
    window.yMax += 10;

    pViewportDrawingArea->queue_draw();
}

void SGI::on_down_button_clicked()
{
    window.yMin -= 10;
    window.yMax -= 10;

    pViewportDrawingArea->queue_draw();
}

void SGI::on_left_button_clicked()
{
    window.xMin -= 10;
    window.xMax -= 10;

    pViewportDrawingArea->queue_draw();
}

void SGI::on_right_button_clicked()
{
    window.xMin += 10;
    window.xMax += 10;

    pViewportDrawingArea->queue_draw();
}

void SGI::on_in_button_clicked()
{
    window.xMax /= 1 + std::stod(pStepEntry->get_buffer()->get_text()) / 100.0;
    window.yMax /= 1 + std::stod(pStepEntry->get_buffer()->get_text()) / 100.0;

    pViewportDrawingArea->queue_draw();
}

void SGI::on_out_button_clicked()
{
    window.xMax *= 1 + std::stod(pStepEntry->get_buffer()->get_text()) / 100.0;
    window.yMax *= 1 + std::stod(pStepEntry->get_buffer()->get_text()) / 100.0;

    pViewportDrawingArea->queue_draw();
}

bool SGI::on_viewport_drawing_area_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{ 
    for (auto p : displayFile.points) {
        draw_point(cr, p);
    }

    for (auto l : displayFile.lines) {
        draw_line(cr, l);
    }

    for (auto w : displayFile.wireframes) {
        draw_wireframe(cr, w);
    }

    cr->stroke();

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

void SGI::draw_point(const Cairo::RefPtr<Cairo::Context>& cr, const Point &p)
{
    auto vpCoordinate = mapToViewport(p.coordinate);

    double x = vpCoordinate.x;
    double y = vpCoordinate.y;

    cr->move_to(x-2, y-2);
    cr->line_to(x+2, y-2);
    cr->line_to(x+2, y+2);
    cr->line_to(x-2, y+2);
    cr->fill();
}

void SGI::draw_line(const Cairo::RefPtr<Cairo::Context>& cr, const Line &l)
{
    auto vpCoordinate_a = mapToViewport(l.coordinate_a);
    auto vpCoordinate_b = mapToViewport(l.coordinate_b);

    cr->move_to(vpCoordinate_a.x, vpCoordinate_a.y);
    cr->line_to(vpCoordinate_b.x, vpCoordinate_b.y);
}

void SGI::draw_wireframe(const Cairo::RefPtr<Cairo::Context>& cr, const Wireframe &w)          
{
    auto firstVpCoordinate = mapToViewport(w.coordinates[0]);

    cr->move_to(firstVpCoordinate.x, firstVpCoordinate.y);

    for (int i = 1; i < w.coordinates.size(); ++i) {
        auto vpCoordinate = mapToViewport(w.coordinates[i]);
        cr->line_to(vpCoordinate.x, vpCoordinate.y);
    }

    cr->line_to(firstVpCoordinate.x, firstVpCoordinate.y);
} 

Coordinate SGI::mapToViewport(const Coordinate &c)
{
    double x = (c.x - window.xMin) * xVpMax / (window.xMax - window.xMin);
    double y = (1 - (c.y - window.yMin)  / (window.yMax - window.yMin)) * yVpMax;

    return Coordinate(x, y);    
}

Coordinate SGI::centerOfLine(const Line &l)
{
    double cx = (l.coordinate_a.x + l.coordinate_b.x) / 2;
    double cy = (l.coordinate_a.x + l.coordinate_b.x) / 2;

    return Coordinate(cx, cy);
}

Coordinate SGI::centerOfWireframe(const Wireframe &w)
{
    double cx = 0;
    double cy = 0;

    for (int i = 0; i < w.coordinates.size(); ++i) {
        cx += w.coordinates[i].x;
        cy += w.coordinates[i].y;
    }

    cx /= w.coordinates.size();
    cy /= w.coordinates.size();

    return Coordinate(cx, cy);
}

Coordinate SGI::applyMatrixOnCoordinate(const Coordinate &c, const Matrix &m)
{   
    double x = c.x * m.v[0][0] + c.y * m.v[1][0] + m.v[2][0];
    double y = c.x * m.v[0][1] + c.y * m.v[1][1] + m.v[2][1];

    return Coordinate(x, y);
}

Line SGI::applyMatrixOnLine(const Line &l, const Matrix &m)
{
    Coordinate c1 = applyMatrixOnCoordinate(l.coordinate_a, m);
    Coordinate c2 = applyMatrixOnCoordinate(l.coordinate_b, m);

    return Line(l.name, c1, c2);
}

Wireframe SGI::applyMatrixOnWireframe(const Wireframe &w, const Matrix &m)
{
    std::vector<Coordinate> coordinates;

    for (int i = 0; i < w.coordinates.size(); ++i) {
        coordinates.push_back(applyMatrixOnCoordinate(w.coordinates[i], m));
    }

    return Wireframe(w.name, coordinates);
}

Point SGI::translatePoint(const Point &p, double dx, double dy)
{
    return Point(p.name, applyMatrixOnCoordinate(p.coordinate, Matrix::translation(dx, dy)));
}

Line SGI::translateLine(const Line &l, double dx, double dy)
{
    return applyMatrixOnLine(l, Matrix::translation(dx, dy));
}

Wireframe SGI::translateWireframe(const Wireframe &w, double dx, double dy)
{
    return applyMatrixOnWireframe(w, Matrix::translation(dx, dy));
}

Line SGI::scaleLine(const Line &l, double sx, double sy)
{
    Coordinate center = centerOfLine(l);

    return applyMatrixOnLine(l, Matrix::scaling(center.x, center.y, sx, sy));
}

Wireframe SGI::scaleWireframe(const Wireframe &w, double sx, double sy)
{
    Coordinate center = centerOfWireframe(w);

    return applyMatrixOnWireframe(w, Matrix::scaling(center.x, center.y, sx, sy));
}

Line SGI::rotateLine(const Line &l, double degrees)
{
    return rotateLine(l, centerOfLine(l), degrees);
}

Line SGI::rotateLine(const Line &l, const Coordinate &c, double degrees)
{
    return applyMatrixOnLine(l, Matrix::rotation(c.x, c.y, degrees));
}

Wireframe SGI::rotateWireframe(const Wireframe &w, double degrees)
{
    return rotateWireframe(w, centerOfWireframe(w), degrees);
}

Wireframe SGI::rotateWireframe(const Wireframe &w, const Coordinate &c, double degrees)
{
    return applyMatrixOnWireframe(w, Matrix::rotation(c.x, c.y, degrees));
}