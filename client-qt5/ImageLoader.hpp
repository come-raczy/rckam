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

#ifndef DEVICES_IMAGE_LOADER_HPP
#define DEVICES_IMAGE_LOADER_HPP

#include <string>
#include <thread>
#include <boost/asio.hpp>

#include "client/ImagePreview.hpp"

namespace rckam
{
namespace client
{

/**
 ** \brief component in charge of capturing the image previews
 **
 ** Reads the data from the socket and use it to initialize a QImage instance
 ** with QImage::loadFromData.
 ** QImage::loadFromData(const uchar *data, int len, const char *format = nullptr)
 ** Note: only supports jpg
 ** Once the QImage instance is initialized, passes it to ImagePreview
 ** TODO: add image format detection and verification
 **/
class ImageLoader
{
public:
  ImageLoader(client::ImagePreview *imagePreview, const std::string ipAddress, unsigned port);
  ~ImageLoader();
  void stop() {stop_ = true;}
  void start();
private:
  client::ImagePreview *imagePreview_;
  boost::asio::io_service ioService_;
  boost::asio::ip::tcp::socket socket_;
  std::string ipAddress_;
  unsigned dataPort_;
  std::thread thread_;
  std::exception_ptr threadException_;
  bool stop_;
  // read the images from the socket and load them into the viewer
  void read();
  // wrap the read method to propagate exceptions is any
  void readWrapper();
};

} // namespace client
} // namespace rckam

#endif // #ifndef DEVICES_IMAGE_LOADER_HPP
