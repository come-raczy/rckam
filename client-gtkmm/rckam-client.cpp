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

#include "common/Debug.hpp"
#include "common/Exceptions.hpp"
#include "client-gtkmm/RckamOptions.hpp"

Gtk::Dialog *pDialog = nullptr;
void rckamGui(const rckam::client::RckamOptions &options);
static void on_button_clicked()
{
  if(pDialog)
    pDialog->hide(); //hide() will cause main::run() to end.
}

int main(int argc, char *argv[])
{
  rckam::common::run(rckamGui, argc, argv);
}

void rckamGui(const rckam::client::RckamOptions &options)
{
  RCKAM_THREAD_CERR << "INFO: rckam-client " << options.description << std::endl;
  const Glib::ustring APPLICATION_IDENTIFIER("rckam.client-gtkmm.rckam-client");
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
}

