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

#include "client/ImagePreview.hpp"

#include <fstream>
#include <vector>
#include <filesystem>

#include <gdkmm/pixbufloader.h>

#include "common/Debug.hpp"

namespace rckam
{
namespace client
{

ImagePreview::ImagePreview(GtkImage *baseObject, const Glib::RefPtr<Gtk::Builder> &)
: Gtk::Image(baseObject)
{
}

void ImagePreview::set(const char *data, size_t size)
{
  auto pixbufLoader = Gdk::PixbufLoader::create();
  pixbufLoader->write(reinterpret_cast<const guint8*>(data), size);
  pixbufLoader->close();
  Gtk::Image::set(pixbufLoader->get_pixbuf());
}

} // namespace client
} // namespace rckam

