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

#include <cassert>
#include <gtkmm/application.h>
#include <gtkmm/builder.h>
#include <gtkmm/dialog.h>
#include <gtkmm/icontheme.h>

#include "common/Debug.hpp"
#include "common/Exceptions.hpp"
#include "client/RckamOptions.hpp"
#include "client/Application.hpp"
#include "client/MainWindow.hpp"
#include "client/ImagePreview.hpp"

void rckamClient(const rckam::client::RckamOptions &options);

int main(int argc, char *argv[])
{
  rckam::common::run(rckamClient, argc, argv);
}

void rckamClient(const rckam::client::RckamOptions &options)
{
  using rckam::client::Application;
  Glib::RefPtr<Application> application = Application::create();
  Glib::RefPtr<Gtk::IconTheme> iconTheme = Gtk::IconTheme::get_default();
  iconTheme->add_resource_path("/resources/icons/feather");
  auto builder = Gtk::Builder::create();
  try
  {
    builder->add_from_resource("/resources/rckam.glade");
  }
  catch(const Glib::Error& ex)
  {
    RCKAM_THREAD_CERR << "ERROR: Building menus and toolbar failed: " <<  ex.what() << std::endl;
    BOOST_THROW_EXCEPTION(rckam::common::GtkmmException(ex.code(), ex.what()));
  }
  rckam::client::MainWindow *mainWindow = nullptr;
  builder->get_widget_derived("mainWindow", mainWindow);
  if (!mainWindow)
  {
    RCKAM_THREAD_CERR << "ERROR: failed to find mainWindow" << std::endl;
    BOOST_THROW_EXCEPTION(rckam::common::GtkmmException(ENOENT, "failed to find mainWindow"));
  }
  rckam::client::ImagePreview *imagePreview = nullptr;
  builder->get_widget_derived("imagePreview", imagePreview);
  if (!imagePreview)
  {
    RCKAM_THREAD_CERR << "ERROR: failed to find imagePreview" << std::endl;
    BOOST_THROW_EXCEPTION(rckam::common::GtkmmException(ENOENT, "failed to find imagePreview"));
  }
  // TODO: start the image loader thread
  // signals must be connected manually with gtkmm and preferably in the constructors of the relevant classes.
  // Alternately, compile with -rdynamic and use "gtk_builder_connect_signals(builder->gobj(), NULL)" but
  // this restricts the callbacks to functions only - no class methods
  try
  {
    application->run(*mainWindow);
  }
  catch(rckam::common::ExceptionData &exception)
  {
    RCKAM_THREAD_CERR << "ERROR: exception caught: " << exception.getContext() << ": " << exception.getErrorNumber() << ": " << exception.getMessage() << std::endl;
  }
  catch(std::exception &exception)
  {
    RCKAM_THREAD_CERR << "ERROR: exception caught: " << exception.what() << std::endl;
  }
  catch(...)
  {
    RCKAM_THREAD_CERR << "ERROR: unknown exception" << std::endl;
  }
  delete mainWindow;
  delete imagePreview;
}

