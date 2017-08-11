#include <iostream>

#include <gtkmm.h>
#include <cairomm/context.h>

#include "drawable.h"
#include "viewport.h"

using namespace std;

Gtk::Window* pMainWindow = nullptr;

Gtk::DrawingArea* pViewportDrawingArea = nullptr;

Gtk::Entry* pStepEntry = nullptr;

Gtk::Button* pUpButton = nullptr;
Gtk::Button* pDownButton = nullptr;
Gtk::Button* pLeftButton = nullptr;
Gtk::Button* pRightButton = nullptr;
Gtk::Button* pInButton = nullptr;
Gtk::Button* pOutButton = nullptr;

Viewport* viewport = nullptr;

static
void on_up_button_clicked()
{
  viewport->x -= 10;
  pViewportDrawingArea->queue_draw();
}

void on_down_button_clicked()
{
  viewport->x += 10;
  pViewportDrawingArea->queue_draw();
}

void on_left_button_clicked()
{
  viewport->y -= 10;
  pViewportDrawingArea->queue_draw();
}

void on_right_button_clicked()
{
  viewport->y -= 10;
  pViewportDrawingArea->queue_draw();
}

void on_in_button_clicked()
{
  viewport->scale += 0.1;
  pViewportDrawingArea->queue_draw();
}

void on_out_button_clicked()
{
  viewport->scale -= 0.1;
  pViewportDrawingArea->queue_draw();
}

bool on_viewport_drawing_area_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  Gtk::Allocation allocation = pViewportDrawingArea->get_allocation();

  auto x_center = allocation.get_width() / 2;
  auto y_center = allocation.get_height() / 2;

  for(auto drawable : viewport->drawables){
    // drawable.draw(cr);

    double x = x_center + drawable.coordinate.x;
    double y = y_center + drawable.coordinate.y;

    cr->set_source_rgb(0, 0, 0);
    cr->move_to(x-5, y-5);
    cr->line_to(x+5, y-5);
    cr->line_to(x+5, y+5);
    cr->line_to(x-5, y+5);
    cr->fill();
  }

  return true;
}

int main (int argc, char **argv)
{
  viewport = new Viewport();
  viewport->drawables.push_back(Point(Coordinate(-50, -50)));
  viewport->drawables.push_back(Point(Coordinate(50, -50)));
  viewport->drawables.push_back(Point(Coordinate(0, 0)));
  viewport->drawables.push_back(Point(Coordinate(-50, 50)));
  viewport->drawables.push_back(Point(Coordinate(50, 50)));

  auto app = Gtk::Application::create(argc, argv, "br.ufsc.ine5420.sgi");

  //Load the GtkBuilder file and instantiate its widgets:
  auto refBuilder = Gtk::Builder::create();
  try
  {
    refBuilder->add_from_file("sgi.glade");
  }
  catch(const Glib::FileError& ex)
  {
    std::cerr << "FileError: " << ex.what() << std::endl;
    return 1;
  }
  catch(const Glib::MarkupError& ex)
  {
    std::cerr << "MarkupError: " << ex.what() << std::endl;
    return 1;
  }
  catch(const Gtk::BuilderError& ex)
  {
    std::cerr << "BuilderError: " << ex.what() << std::endl;
    return 1;
  }

  refBuilder->get_widget("MainWindow", pMainWindow);
  if(pMainWindow)
  {
    refBuilder->get_widget("ViewportDrawingArea", pViewportDrawingArea);
    if(pViewportDrawingArea)
    {
      pViewportDrawingArea->signal_draw().connect( sigc::ptr_fun(on_viewport_drawing_area_draw) );
    }

    refBuilder->get_widget("StepEntry", pStepEntry);

    refBuilder->get_widget("UpButton", pUpButton);
    if(pUpButton)
    {
      pUpButton->signal_clicked().connect( sigc::ptr_fun(on_up_button_clicked) );
    }

    refBuilder->get_widget("DownButton", pDownButton);
    if(pDownButton)
    {
      pDownButton->signal_clicked().connect( sigc::ptr_fun(on_down_button_clicked) );
    }

    refBuilder->get_widget("LeftButton", pLeftButton);
    if(pLeftButton)
    {
      pLeftButton->signal_clicked().connect( sigc::ptr_fun(on_left_button_clicked) );
    }

    refBuilder->get_widget("RightButton", pRightButton);
    if(pRightButton)
    {
      pRightButton->signal_clicked().connect( sigc::ptr_fun(on_right_button_clicked) );
    }

    refBuilder->get_widget("InButton", pInButton);
    if(pInButton)
    {
      pInButton->signal_clicked().connect( sigc::ptr_fun(on_in_button_clicked) );
    }

    refBuilder->get_widget("OutButton", pOutButton);
    if(pOutButton)
    {
      pOutButton->signal_clicked().connect( sigc::ptr_fun(on_out_button_clicked) );
    }

    app->run(*pMainWindow);
  }

  delete pMainWindow;
  delete viewport;

  return 0;
}