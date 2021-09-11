/**
 ** rckam
 ** Copyright (c) 2020-2021 Come Raczy
 ** All rights reserved.
 **
 ** This software is provided under the terms and conditions of the
 ** GNU AFFERO GENERAL PUBLIC LICENSE
 **
 ** You should have received a copy of the
 ** GNU AFFERO GENERAL PUBLIC LICENSE
 ** along with this program. If not, see
 ** <https://fsf.org/>
 **/

#include "client/Application.hpp"

#include "common/Debug.hpp"

namespace rckam
{
namespace client
{

Application::Application()
: Gtk::Application("rckam.client.application", Gio::APPLICATION_HANDLES_OPEN)
{
}

Glib::RefPtr<Application> Application::create()
{
  return Glib::RefPtr<rckam::client::Application>(new Application());
}

int Application::run(Gtk::Window &window)
{
  return  Gtk::Application::run(window);
}

void Application::on_activate()
{
  // The application has been started, so let's show a window.
  //auto rckamMainWindow = createMainWindow();
  //rckamMainWindow->present();
}

void Application::on_open(const Gio::Application::type_vec_files& files, const Glib::ustring& hint)
{
  // The application has been asked to open some files, so let's open a new view for each one.
  RCKAM_THREAD_CERR << "INFO: Application::on_open(...)" << std::endl;
#if 0
  Window* appwindow = nullptr;
  auto windows = get_windows();
  if (windows.size() > 0)
    appwindow = dynamic_cast<ExampleAppWindow*>(windows[0]);

  if (!appwindow)
    appwindow = createMainWindow();

  for (const auto& file : files)
    appwindow->open_file_view(file);

  appwindow->present();
#endif
}

//RckamMainWindow* Application::createMainWindow()
//{
  //auto rckamMainWindow = new RckamMainWindow();
  // make sure the application runs as long as this window is open
  // Gtk::Application::add_window() connects a signal handler to the window's
  // signal_hide(). That handler removes the window from the application.
  // If it's the last window to be removed, the application stops running.
  // Gtk::Window::set_application() does not connect a signal handler, but is
  // otherwise equivalent to Gtk::Application::add_window().
  //add_window(*rckamMainWindow);
  // Delete the window when it is hidden.
  //rckamMainWindow->signal_hide().connect(sigc::bind<Gtk::Window*>(sigc::mem_fun(*this,&Application::on_hide_window), rckamMainWindow));
  //return rckamMainWindow;
  //return nullptr;
//}

void Application::on_hide_window(Gtk::Window* window)
{
  delete window;
}

} // namespace client
} // namespace rckam

