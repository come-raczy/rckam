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

#ifndef RCKAM_CLIENT_RCKAM_APPLICATION_HPP
#define RCKAM_CLIENT_RCKAM_APPLICATION_HPP

#include <gtkmm/application.h>

#include "client/RckamMainWindow.hpp"

namespace rckam
{
namespace client
{

class RckamApplication: public Gtk::Application
{
protected:
  RckamApplication();
public:
  static Glib::RefPtr<RckamApplication> create();
  int run(Gtk::Window &window);
protected:
  // Override default signal handlers:
  void on_activate() override;
  void on_open(const Gio::Application::type_vec_files& files, const Glib::ustring& hint) override;
private:
//  RckamMainWindow* createMainWindow();
  void on_hide_window(Gtk::Window* window);
};

} // namespace client
} // namespace rckam

#endif // #ifndef RCKAM_CLIENT_RCKAM_APPLICATION_HPP

