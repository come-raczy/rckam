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

#include <string>

#include <gtkmm/applicationwindow.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/builder.h>
#include <gtkmm/enums.h> // for IconSize
#include <glibmm/refptr.h>
#include <giomm/file.h>

#include "client/ImagePreview.hpp"
#include "client/ImageLoader.hpp"

namespace rckam
{
namespace client
{

class MainWindow: public Gtk::ApplicationWindow
{
public:
  MainWindow(GtkApplicationWindow *baseObject, const Glib::RefPtr<Gtk::Builder> &builder);
  ~MainWindow();
  void open_file_view(const Glib::RefPtr<Gio::File>& file);
  void setServer(const std::string &ipAddress, const unsigned dataPort);
private:
  ImagePreview *imagePreview_;
  ImageLoader imageLoader_;
  std::string ipAddress_;
  Gtk::Statusbar *statusBar_;
  unsigned dataPort_;
  void on_quitMenuItem_activate() {hide();}
  void on_startStopPreviewToggleButton_toggle(Gtk::ToggleButton* toggleButton);
  static constexpr auto START_STOP_ICON_SIZE = Gtk::ICON_SIZE_DIALOG;
};

} // namespace client
} // namespace rckam

#endif // #ifndef RCKAM_CLIENT_MAIN_WINDOW_HPP

