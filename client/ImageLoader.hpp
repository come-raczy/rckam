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

#ifndef RCKAM_CLIENT_IMAGE_LOADER_HPP
#define RCKAM_CLIENT_IMAGE_LOADER_HPP

#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <boost/asio.hpp>

#include "ImagePreview.hpp"

namespace rckam
{
namespace client
{

/**
 ** \brief load images and send them to the ImagePreview
 **
 ** Preview images are acquired on their own thread. It is the responsibility
 ** of the ImagePreview component to notify the GUI thread that an update is
 ** required.
 **
 ** TODO: add communication, coordination and synchronization with remote
 **/
class ImageLoader
{
public:
  ImageLoader(ImagePreview &imagePreview, const std::string &ipAddress, unsigned port);
  ~ImageLoader();
  /// start the preview stream
  void start();
  /// notifies threads for streaming previews to stop
  void stop();
  /// check if any of the associated threads is stilll executing
  bool isRunning() {return readPreviewsThread_.joinable() || setPreviewsThread_.joinable();}
private:
  ImagePreview *imagePreview_;
  //boost::asio::io_service ioService_;
  boost::asio::io_context ioContext_;
  boost::asio::ip::udp::socket socket_;
  std::string ipAddress_;
  unsigned dataPort_;
  boost::asio::ip::udp::endpoint remoteEndpoint_;
  std::thread readPreviewsThread_;
  std::thread setPreviewsThread_;
  std::exception_ptr readPreviewsThreadException_;
  std::exception_ptr setPreviewsThreadException_;
  std::array<std::vector<char>, 2> buffers_;
  // controls thread access to each of the buffers
  std::array<std::mutex, 2> mutexes_;
  // enable thread synchronization over buffer acquisition
  std::array<std::condition_variable, 2> conditionVariables_;
  // state of the buffers
  std::array<bool, 2> buffersEmpty_;
  // mutex for the whole instance for start and stop control
  std::mutex mutex_;
  bool stop_;
  /// loop that reads the image previews as they are produced on the socket
  void readPreviews();
  /// wrapper for exception management
  void readPreviewsWrapper();
  /// read a single image preview over the socket
  void readPreview(std::vector<char> &data);
  /// loop setting the image previews into the GUI
  void setPreviews();
  /// wrapper for exception management
  void setPreviewsWrapper();
};

} // namespace client
} // namespace rckam

#endif // #ifndef RCKAM_CLIENT_IMAGE_LOADER_HPP

