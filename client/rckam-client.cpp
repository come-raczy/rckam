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
#include "client/RckamApplication.hpp"

void rckamClient(const rckam::client::RckamOptions &options);

int main(int argc, char *argv[])
{
  rckam::common::run(rckamClient, argc, argv);
}

void rckamClient(const rckam::client::RckamOptions &options)
{
  using rckam::client::RckamApplication;
  Glib::RefPtr<RckamApplication> rckamApplication = RckamApplication::create();
  //Glib::RefPtr<Gtk::Application> rckamApplication = Gtk::Application::create("org.gtkmm.example");
  Glib::RefPtr<Gtk::IconTheme> iconTheme = Gtk::IconTheme::get_default();
  iconTheme->add_resource_path("/resources/icons/feather");
  auto builder = Gtk::Builder::create();
  try
  {
    builder->add_from_resource("/resources/rckam.glade");
  }
  catch(const Glib::Error& ex)
  {
    RCKAM_THREAD_CERR << "Building menus and toolbar failed: " <<  ex.what() << std::endl;
    BOOST_THROW_EXCEPTION(rckam::common::GtkmmException(ex.code(), ex.what()));
  }
  rckam::client::RckamMainWindow *rckamMainWindow = nullptr;
  builder->get_widget_derived("rckamMainWindow", rckamMainWindow);
  if(rckamMainWindow)
  {
    rckamApplication->run(*rckamMainWindow);
    delete rckamMainWindow;
  }
  else
  {
    RCKAM_THREAD_CERR << "ERROR: failed to find rckamMainWindow" << std::endl;
  }


#if 0
  // go the programatic route
  auto rckamApplication = rckam::client::RckamApplication::create();
  const auto ret = rckamApplication->run(options);
  if (0 != ret)
  {
    RCKAM_THREAD_CERR << "WARNING: rckam application returned an error: " << ret << std::endl;
  }
#endif
  // go the glade route
  // use glade as usual for the layout and then use the derived classes


#if 0
  RCKAM_THREAD_CERR << "INFO: rckam-client " << options.description << std::endl;
  const Glib::ustring APPLICATION_IDENTIFIER("rckam.client.rckam-client");
  assert(Gio::Application::id_is_valid(APPLICATION_IDENTIFIER));
  auto app = Gtk::Application::create(APPLICATION_IDENTIFIER);
  RCKAM_THREAD_CERR << "INFO: loading rckam.glade" << std::endl;
  auto refBuilder = Gtk::Builder::create();
  try
  {
    //refBuilder->add_from_string(ui_info);
    refBuilder->add_from_resource("/resources/rckam.glade");
  }
  catch(const Glib::Error& ex)
  {
    RCKAM_THREAD_CERR << "ERROR: Building menus and toolbar failed: " <<  ex.what() << std::endl;
    BOOST_THROW_EXCEPTION(rckam::common::GtkmmException(ex.code(), ex.what()));
  }
  refBuilder->get_widget("DialogBasic", pDialog);
  if(pDialog)
  {
    //Get the GtkBuilder-instantiated Button, and connect a signal handler:
    Gtk::Button* pButton = nullptr;
    refBuilder->get_widget("quit_button", pButton);
    if(pButton)
    {
      pButton->signal_clicked().connect( sigc::ptr_fun(on_button_clicked) );
    }
    app->run(*pDialog);
  }
  delete pDialog;
#endif
}

