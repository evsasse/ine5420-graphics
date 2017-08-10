#include <gtkmm.h>
#include <iostream>

Gtk::Window* pHelloWorldWindow = nullptr;
// Gtk::Window* pAnotherWindow = nullptr;

static
void on_button_clicked()
{
  if(pHelloWorldWindow){
  	std::cout << "Bye World!\n";
    pHelloWorldWindow->hide(); //hide() will cause main::run() to end.
  }
}

int main (int argc, char **argv)
{
  auto app = Gtk::Application::create(argc, argv, "br.ufsc.ine5420.sgi");

  //Load the GtkBuilder file and instantiate its widgets:
  auto refBuilder = Gtk::Builder::create();
  try
  {
    refBuilder->add_from_file("helloworld.glade");
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

  //Get the GtkBuilder-instantiated Window:
  refBuilder->get_widget("HelloWorldWindow", pHelloWorldWindow);
  if(pHelloWorldWindow)
  {
    //Get the GtkBuilder-instantiated Button, and connect a signal handler:
    Gtk::Button* pButton = nullptr;
    refBuilder->get_widget("button1", pButton);
    if(pButton)
    {
      pButton->signal_clicked().connect( sigc::ptr_fun(on_button_clicked) );
    }

    std::cout << "Hello World!\n";

    app->run(*pHelloWorldWindow);
  }

  delete pHelloWorldWindow;

  return 0;
}