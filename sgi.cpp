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

    displayFile.points.push_back(Point("asd", Coordinate(20, 20)));

    displayFile.lines.push_back(Line("asd", Coordinate(150, 150), Coordinate(200, 250)));

    std::vector<Coordinate> coordinates;
    coordinates.push_back(Coordinate(50, 50));
    coordinates.push_back(Coordinate(50, 100));
    coordinates.push_back(Coordinate(100, 100));
    coordinates.push_back(Coordinate(100, 50));

    displayFile.wireframes.push_back(Wireframe("asd", coordinates));
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