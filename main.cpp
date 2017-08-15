#include <iostream>

#include <gtkmm.h>
#include <cairomm/context.h>

//#include "drawable.h"
//#include "viewport.h"

#include "sgi.h"

using namespace std;



int main (int argc, char **argv)
{
  auto app = Gtk::Application::create(argc, argv, "br.ufsc.ine5420.sgi");

  auto builder = Gtk::Builder::create_from_file("sgi.glade");
  SGI *sgi;
  builder->get_widget_derived("MainWindow", sgi);
  app->run(*sgi);

  return 0;
}