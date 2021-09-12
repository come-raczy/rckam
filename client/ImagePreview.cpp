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

#include "common/Debug.hpp"

namespace rckam
{
namespace client
{

ImagePreview::ImagePreview(GtkImage *baseObject, const Glib::RefPtr<Gtk::Builder> &)
: Gtk::Image(baseObject)
, pixbufLoader_(Gdk::PixbufLoader::create("jpeg"))
{
  //set("/tmp/preview0000.jpg");
  std::filesystem::path filePath = "/tmp/preview0000.jpg";
  const auto fileSize = file_size(filePath);
  std::vector<char> data(fileSize);
  std::ifstream is(filePath);
  is.read(data.data(), fileSize);
  if (is.gcount() != static_cast<std::streamsize>(fileSize))
  {
    RCKAM_THREAD_CERR << "ERROR: failed to read image" << std::endl;
  }
  const auto formats =  Gdk::Pixbuf::get_formats();
  for(const auto &f: formats)
  {
    std::cerr << f.get_name() << ": " << f.get_description() << std::endl;
  }
  try
  {
    //auto pixbufLoader =  Gdk::PixbufLoader::create("jpeg"); // as known by PixbufFormat
    pixbufLoader_->write(reinterpret_cast<const guint8*>(data.data()), fileSize);
    pixbufLoader_->close();
    Gtk::Image::set(pixbufLoader_->get_pixbuf());
  }
  catch(Gdk::PixbufError &e)
  {
    RCKAM_THREAD_CERR << "ERROR: " << e.code() << ": " << e.what() << std::endl;
  }
}

void ImagePreview::set(const char *data, size_t size)
{
  pixbufLoader_->write(reinterpret_cast<const guint8*>(data), size);
  pixbufLoader_->close();
  Gtk::Image::set(pixbufLoader_->get_pixbuf());
  // TODO: notify GUI event loop
}

void ImagePreview::open_file_view(const Glib::RefPtr<Gio::File>& /* file */)
{
}

} // namespace client
} // namespace rckam

