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

#ifndef RCKAM_CLIENT_MAIN_WINDOW_HPP
#define RCKAM_CLIENT_MAIN_WINDOW_HPP

#include <gtkmm/applicationwindow.h>
#include <gtkmm/builder.h>
#include <glibmm/refptr.h>
#include <giomm/file.h>

namespace rckam
{
namespace client
{

class MainWindow: public Gtk::ApplicationWindow
{
public:
  MainWindow(GtkApplicationWindow *baseObject, const Glib::RefPtr<Gtk::Builder> &builder);
  void open_file_view(const Glib::RefPtr<Gio::File>& file);
private:
  //Glib::RefPtr<Gtk::Builder> builder_; 
  void on_quitMenuItem_activate() {hide();}
};

} // namespace client
} // namespace rckam

#endif // #ifndef RCKAM_CLIENT_MAIN_WINDOW_HPP
