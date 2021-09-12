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

#include "ImageLoader.hpp"

#include "common/Debug.hpp"

namespace rckam
{
namespace client
{

ImageLoader::ImageLoader(ImagePreview &imagePreview)
: imagePreview_(&imagePreview)
, stop_(true)
{
}

void ImageLoader::acquireImage(std::vector<char> &data)
{
}

ImageLoader::~ImageLoader()
{
  stop();
  if (thread_.joinable())
  {
    thread_.join();
  }
}

void ImageLoader::start()
{
  // do some initialization
  stop_ = false;
  thread_ = std::thread(&ImageLoader::runWrapper, this);
}

void ImageLoader::runWrapper()
{
  try
  {
    run();
  }
  catch(std::exception &e)
  {
    RCKAM_THREAD_CERR << "WARNING: ImageLoader thread terminated: " << e.what() << std::endl;
    threadException_ = std::current_exception();
  }
}

void ImageLoader::run()
{
  std::vector<char> data;
  while (!stop_)
  {

    acquireImage(data);
    // notify ImagePreview
  }
}

} // namespace client
} // namespace rckam

