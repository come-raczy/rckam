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
, remoteEndpoint_(boost::asio::ip::make_address_v4(ipAddress), dataPort)
, readPreviewsThread_()
, setPreviewsThread_()
, readPreviewsThreadException_(nullptr)
, setPreviewsThreadException_(nullptr)
, buffersEmpty_{true, true}
, stop_(true)
{
}

void ImageLoader::readPreview(std::vector<char> &data)
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
  constexpr boost::asio::socket_base::message_flags FLAGS = 0;
  using common::RckamException;
  std::array<char, sizeof(size_t)> byteCountBuffer;
  boost::system::error_code error;
  // read the size of the image in bytes
  // TCP
  // const size_t count1 = boost::asio::read(socket_, boost::asio::buffer(byteCountBuffer), boost::asio::transfer_exactly(sizeof(size_t)), error);
  // UDP
  boost::asio::ip::udp::endpoint senderEndpoint;
  const size_t count1 = socket_.receive_from(boost::asio::buffer(byteCountBuffer), senderEndpoint, FLAGS, error);
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
  // TCP
  // const size_t count2 = boost::asio::read(socket_, boost::asio::buffer(data), boost::asio::transfer_exactly(byteCount), error);
  // UDP
  const size_t count2 = socket_.receive_from(boost::asio::buffer(data), senderEndpoint, FLAGS, error);
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
  if (readPreviewsThread_.joinable())
  {
    readPreviewsThread_.join();
  }
  if (setPreviewsThread_.joinable())
  {
    setPreviewsThread_.join();
  }
}

void ImageLoader::stop()
{
  std::unique_lock lock(mutex_);
  stop_ = true;
  for (auto &c: conditionVariables_)
  {
    c.notify_all();
  }
}

void ImageLoader::start()
{
  constexpr boost::asio::socket_base::message_flags FLAGS = 0;
  assert(!isRunning());
  RCKAM_THREAD_CERR << "INFO: starting image loader..." << std::endl;
  using namespace boost::asio;
  using ip::udp;
  using common::RckamException;
  boost::system::error_code error;
  RCKAM_THREAD_CERR << "INFO: connecting image loader to " <<  ipAddress_ << ":" << dataPort_ << "..." << std::endl;
  // TCP
  //socket_.connect( udp::endpoint( ip::make_address(ipAddress_), dataPort_), error);
  std::array<char, 1> sendBuffer{0};
  socket_.send_to(boost::asio::buffer(sendBuffer), remoteEndpoint_, FLAGS, error);
  if (error)
  {
    //auto message = boost::format("ERROR: failed to connect to data socket '%s:%d' (check that server is ready): %d: %s") % ipAddress_ % dataPort_ % error.value() % error.message();
    auto message = boost::format("ERROR: failed to send to remote socket '%s:%d' (check that server is ready): %d: %s") % ipAddress_ % dataPort_ % error.value() % error.message();
    BOOST_THROW_EXCEPTION(RckamException(message.str()));
  }
  else
  {
    RCKAM_THREAD_CERR << "INFO: connecting image loader to " <<  ipAddress_ << ":" << dataPort_ << "... done" << std::endl;
    RCKAM_THREAD_CERR << "INFO: starting threads..." << std::endl;
    std::unique_lock lock(mutex_);
    stop_ = false;
    for (auto &bufferEmpty: buffersEmpty_)
    {
      bufferEmpty = true;
    }
    readPreviewsThreadException_ = nullptr;
    readPreviewsThread_ = std::thread(&ImageLoader::readPreviewsWrapper, this);
    setPreviewsThreadException_ = nullptr;
    setPreviewsThread_ = std::thread(&ImageLoader::setPreviewsWrapper, this);
    RCKAM_THREAD_CERR << "INFO: starting image loader... done" << std::endl;
  }
}

void ImageLoader::readPreviewsWrapper()
{
  try
  {
    readPreviews();
  }
  catch(std::exception &e)
  {
    RCKAM_THREAD_CERR << "WARNING: ImageLoader thread terminated: " << e.what() << std::endl;
    readPreviewsThreadException_ = std::current_exception();
  }
  catch(...)
  {
    RCKAM_THREAD_CERR << "WARNING: ImageLoader thread terminated: unknown exception" << std::endl;
    readPreviewsThreadException_ = std::current_exception();
  }
}

void ImageLoader::readPreviews()
{
  unsigned i = 0;
  unsigned index = 0;
  while (!stop_)
  {
    RCKAM_THREAD_CERR << "INFO: acquiring empty buffer      " << std::setw(5) << i << "..." << std::endl;
    std::unique_lock<std::mutex> lock(mutexes_[index]);
    conditionVariables_[index].wait(lock, [&] {return buffersEmpty_[index] || stop_;});
    if (stop_)
    {
      break;
    }
    auto &buffer = buffers_[index];
    RCKAM_THREAD_CERR << "INFO: reading preview from socket " << std::setw(5) << i << "..." << std::endl;
    readPreview(buffer);
    RCKAM_THREAD_CERR << "INFO: reading preview from socket " << std::setw(5) << i << "... done" << std::endl;
    buffersEmpty_[index] = false;
    conditionVariables_[index].notify_one();
    index = (index + 1) % 2;
    ++i;
  }
}

void ImageLoader::setPreviewsWrapper()
{
  try
  {
    setPreviews();
  }
  catch(std::exception &e)
  {
    RCKAM_THREAD_CERR << "WARNING: ImageLoader thset terminated: " << e.what() << std::endl;
    setPreviewsThreadException_ = std::current_exception();
  }
  catch(...)
  {
    RCKAM_THREAD_CERR << "WARNING: ImageLoader thset terminated: unknown exception" << std::endl;
    setPreviewsThreadException_ = std::current_exception();
  }
}

void ImageLoader::setPreviews()
{
  unsigned i = 0;
  unsigned index = 0;
  while (!stop_)
  {
    RCKAM_THREAD_CERR << "INFO: acquiring full buffer  " << std::setw(5) << i << "..." << std::endl;
    std::unique_lock<std::mutex> lock(mutexes_[index]);
    conditionVariables_[index].wait(lock, [&] {return (false == buffersEmpty_[index]) || stop_;});
    if (stop_)
    {
      break;
    }
    auto &buffer = buffers_[index];
    RCKAM_THREAD_CERR << "INFO: setting preview in GUI " << std::setw(5) << i << "..." << std::endl;
    imagePreview_->set(buffer.data(), buffer.size());
    RCKAM_THREAD_CERR << "INFO: setting preview in GUI " << std::setw(5) << i << "... done" << std::endl;
    buffersEmpty_[index] = true;
    conditionVariables_[index].notify_one();
    index = (index + 1) % 2;
    ++i;
  }
}

} // namespace client
} // namespace rckam

