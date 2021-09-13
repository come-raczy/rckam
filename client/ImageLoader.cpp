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

#include <chrono>
#include <filesystem>
#include <fstream>
#include <boost/format.hpp>

#include "common/Debug.hpp"
#include "common/Exceptions.hpp"

namespace rckam
{
namespace client
{

ImageLoader::ImageLoader(ImagePreview &imagePreview, const std::string &ipAddress, const unsigned dataPort)
: imagePreview_(&imagePreview)
, ioService_()
, socket_(ioService_)
, ipAddress_(ipAddress)
, dataPort_(dataPort)
, thread_()
, threadException_(nullptr)
, stop_(true)
{
}

void ImageLoader::acquireImage(std::vector<char> &data)
{
#if 0
  static unsigned i = 0;
  std::filesystem::path filePath((boost::format("/tmp/a0%i.jpg") % i).str());
  const auto fileSize = file_size(filePath);
  data.resize(fileSize);
  std::ifstream is(filePath);
  is.read(data.data(), fileSize);
  if (is.gcount() != static_cast<std::streamsize>(fileSize))
  {
    RCKAM_THREAD_CERR << "ERROR: failed to read " << fileSize << " bytes from " << filePath << ": " << errno << std::endl;
  }
  i = (i + 1) % 5;
#endif
  using common::RckamException;
  std::array<char, sizeof(size_t)> byteCountBuffer;
  boost::system::error_code error;
  // read the size of the image in bytes
  const size_t count1 = boost::asio::read(socket_, boost::asio::buffer(byteCountBuffer), boost::asio::transfer_exactly(sizeof(size_t)), error);
  if (error)
  {
    auto message = boost::format("ERROR: failed to read byte count from data socket: %i: %s") % error.value() % error.message();
    BOOST_THROW_EXCEPTION(RckamException(message.str()));
  }
  else if (sizeof(size_t) != count1)
  {
    auto message = boost::format("ERROR: failed to read %i bytes for byte count from data socket: read only %i") % sizeof(size_t) % count1;
    BOOST_THROW_EXCEPTION(RckamException(message.str()));
  }
  const size_t byteCount = *(reinterpret_cast<size_t *>(byteCountBuffer.data()));
  RCKAM_THREAD_CERR << "INFO: cout1 = " << std::setw(4) << count1 << " byteCount = " <<  std::setw(8) << byteCount << std::endl;
  data.resize(byteCount);
  // read the image
  const size_t count2 = boost::asio::read(socket_, boost::asio::buffer(data), boost::asio::transfer_exactly(byteCount), error);
  if (error)
  {
    auto message = boost::format("ERROR: failed to read image data from data socket: %i: %s") % error.value() % error.message();
    BOOST_THROW_EXCEPTION(RckamException(message.str()));
  }
  else if (byteCount != count2)
  {
    auto message = boost::format("ERROR: failed to read %i bytes for image from data socket: read only %i") % byteCount % count2;
    BOOST_THROW_EXCEPTION(RckamException(message.str()));
  }
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
  assert(!isRunning());
  RCKAM_THREAD_CERR << "INFO: starting image loader..." << std::endl;
  threadException_ = nullptr;
  using namespace boost::asio;
  using ip::tcp;
  using common::RckamException;
  boost::system::error_code error;
  RCKAM_THREAD_CERR << "INFO: connecting image loader to " <<  ipAddress_ << ":" << dataPort_ << "..." << std::endl;
  socket_.connect( tcp::endpoint( ip::make_address(ipAddress_), dataPort_), error);
  if (error)
  {
    auto message = boost::format("ERROR: failed to connect to data socket '%s:%d' (check that server is ready): %d: %s") % ipAddress_ % dataPort_ % error.value() % error.message();
    BOOST_THROW_EXCEPTION(RckamException(message.str()));
  }
  else
  {
    RCKAM_THREAD_CERR << "INFO: connecting image loader to " <<  ipAddress_ << ":" << dataPort_ << "... done" << std::endl;
    // start the thread to read the data
    RCKAM_THREAD_CERR << "INFO: starting thread..." << std::endl;
    thread_ = std::thread(&ImageLoader::runWrapper, this);
    RCKAM_THREAD_CERR << "INFO: starting image loader... done" << std::endl;
  }
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
  catch(...)
  {
    RCKAM_THREAD_CERR << "WARNING: ImageLoader thread terminated: unknown exception" << std::endl;
    threadException_ = std::current_exception();
  }
}

void ImageLoader::run()
{
  stop_ = false;
  std::vector<char> data;
  while (!stop_)
  {
    RCKAM_THREAD_CERR << "INFO: loading new image..." << std::endl;
    acquireImage(data);
    imagePreview_->set(data.data(), data.size());
    RCKAM_THREAD_CERR << "INFO: loading new image... done" << std::endl;
    //std::this_thread::sleep_for (std::chrono::seconds(1));
  }
}

} // namespace client
} // namespace rckam

