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

#ifndef RCKAM_CLIENT_APPLICATION_HPP
#define RCKAM_CLIENT_APPLICATION_HPP

#include <gtkmm/application.h>
#include <gtkmm/window.h>

#include "RckamClientOptions.hpp"
#include "MainWindow.hpp"
#include "Client.hpp"

namespace rckam
{
namespace client
{

class Application: public Gtk::Application
{
protected:
  Application(const RckamClientOptions &options);
  ~Application();
public:
  static Glib::RefPtr<Application> create(const RckamClientOptions &options);
  int run();
protected:
  // Override default signal handlers:
  void on_activate() override;
  void on_open(const Gio::Application::type_vec_files& files, const Glib::ustring& hint) override;
private:
  void on_hide_window(Gtk::Window* window);
  rckam::client::Client client_;
  rckam::client::MainWindow *mainWindow_;
};

} // namespace client
} // namespace rckam

#endif // #ifndef RCKAM_CLIENT_APPLICATION_HPP

