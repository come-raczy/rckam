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

#ifndef RCKAM_CLIENT_IMAGE_PREVIEW_HPP
#define RCKAM_CLIENT_IMAGE_PREVIEW_HPP

#include <gtkmm/image.h>
#include <gtkmm/builder.h>
#include <glibmm/refptr.h>
#include <giomm/file.h>
#include <gdkmm/pixbufloader.h>

namespace rckam
{
namespace client
{

class ImagePreview: public Gtk::Image
{
public:
  ImagePreview(GtkImage *baseObject, const Glib::RefPtr<Gtk::Builder> &builder);
  void open_file_view(const Glib::RefPtr<Gio::File>& file);
  void set(const char *data, size_t size);
private:
  Glib::RefPtr<Gdk::PixbufLoader> pixbufLoader_;
};

} // namespace client
} // namespace rckam

#endif // #ifndef RCKAM_CLIENT_IMAGE_PREVIEW_HPP

