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

#include "client/RckamMainWindow.hpp"

namespace rckam
{
namespace client
{

RckamMainWindow::RckamMainWindow(GtkApplicationWindow *baseObject, const Glib::RefPtr<Gtk::Builder> &builder)
: Gtk::ApplicationWindow(baseObject)
, builder_(builder)
{
}

void RckamMainWindow::open_file_view(const Glib::RefPtr<Gio::File>& /* file */)
{
}

} // namespace client
} // namespace rckam

