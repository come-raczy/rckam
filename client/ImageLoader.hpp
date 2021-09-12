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
#include <vector>

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
 **/
class ImageLoader
{
public:
  ImageLoader(ImagePreview &imagePreview);
  ~ImageLoader();
  void start();
  void stop() {stop_ = true;}
  bool isRunning() {return thread_.joinable();}
private:
  ImagePreview *imagePreview_;
  std::thread thread_;
  std::exception_ptr threadException_;
  bool stop_;
  void run();
  void runWrapper();
  void acquireImage(std::vector<char> &data);
};

} // namespace client
} // namespace rckam

#endif // #ifndef RCKAM_CLIENT_IMAGE_LOADER_HPP

