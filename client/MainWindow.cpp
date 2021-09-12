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

#include "client/MainWindow.hpp"

#include <gtkmm/menuitem.h>

#include "common/Debug.hpp"
#include "common/Exceptions.hpp"

namespace rckam
{
namespace client
{

MainWindow::MainWindow(GtkApplicationWindow *baseObject, const Glib::RefPtr<Gtk::Builder> &builder)
: Gtk::ApplicationWindow(baseObject)
//, builder_(builder)
{
  Gtk::MenuItem *quitMenuItem = nullptr;
  builder->get_widget("quitMenuItem", quitMenuItem);
  if (!quitMenuItem)
  {
    RCKAM_THREAD_CERR << "ERROR: failed to find quitMenuItem" << std::endl;
    BOOST_THROW_EXCEPTION(rckam::common::GtkmmException(ENOENT, "failed to find quitMenuItem"));
  }
  quitMenuItem->signal_activate().connect(sigc::mem_fun(*this, &rckam::client::MainWindow::on_quitMenuItem_activate));
}

void MainWindow::open_file_view(const Glib::RefPtr<Gio::File>& /* file */)
{
}

} // namespace client
} // namespace rckam

