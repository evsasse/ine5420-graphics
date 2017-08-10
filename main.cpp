#include <gtkmm.h>
#include <iostream>

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

static
void on_up_button_clicked()
{
  // if(pMainWindow){
  // }
  // cout << "ey" << pStepEntry->get_text() << endl;
}

void on_down_button_clicked()
{
  cout << "ey" << endl;
}

void on_left_button_clicked()
{
  cout << "ey" << endl;
}

void on_right_button_clicked()
{
  cout << "ey" << endl;
}

void on_in_button_clicked()
{
  cout << "ey" << endl;
}

void on_out_button_clicked()
{
  cout << "ey" << endl;
}

int main (int argc, char **argv)
{
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

  return 0;
}