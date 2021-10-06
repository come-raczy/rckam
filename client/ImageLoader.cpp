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
#include <boost/system/error_code.hpp> // NOTE: boost::asio does not support std::error_code

#include "common/Debug.hpp"
#include "common/Exceptions.hpp"

namespace rckam
{
namespace client
{

//ImageLoader::ImageLoader(ImagePreview &imagePreview, const std::string &ipAddress, const unsigned dataPort)
ImageLoader::ImageLoader()
//: imagePreview_(&imagePreview)
//, ioService_()
//, socket_(ioService_)
: ioContext_()
, socket_(ioContext_)
//, ipAddress_(ipAddress)
//, dataPort_(dataPort)
//, remoteEndpoint_(boost::asio::ip::make_address_v4(ipAddress), dataPort)
, readPreviewsThread_()
, setPreviewsThread_()
, readPreviewsThreadException_(nullptr)
, setPreviewsThreadException_(nullptr)
, buffersEmpty_{true, true}
, stop_(true)
, currentImageId_(-1)
{
}

/**
 ** The implementation doesn't make any effort to recover from errors (lost packets
 ** and packets out of sequence). When packets are lost, the whole image is discarded.
 ** In that case, the remaining packets for the image are simply skipped until the 
 ** first packet of the subsequent image is received. At that point, the new image
 ** is received normally.
 **/
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
  boost::system::error_code error;
  // read the size of the image in bytes
  // TCP
  // std::array<char, sizeof(size_t)> byteCountBuffer;
  // const size_t count1 = boost::asio::read(socket_, boost::asio::buffer(byteCountBuffer), boost::asio::transfer_exactly(sizeof(size_t)), error);
  // UDP
  boost::asio::ip::udp::endpoint senderEndpoint;
  std::array<char, 4> header; // 2 bytes for image id, 2 bytes for packet id
  uint16_t * const imageId = reinterpret_cast<uint16_t*>(header.data());
  uint16_t * const packetId = reinterpret_cast<uint16_t*>(header.data() + 2);
  bool done = false;
  size_t offset = 0;
  constexpr size_t MAX_PACKET_SIZE = 1472;
  constexpr size_t MAX_PAYLOAD = MAX_PACKET_SIZE - header.size();
  data.resize(MAX_PAYLOAD);
  using boost::asio::mutable_buffer;
  mutable_buffer headerBuffer = mutable_buffer(header.data(), header.size());
  std::array<mutable_buffer, 2> bufferSequence{headerBuffer, mutable_buffer(data.data() + offset, MAX_PAYLOAD)};
  uint16_t nextPacketId = 0;
  bool discard = true; // in case the first packet gets lost
  size_t byteCount = 0;
  while (!done)
  {
    bufferSequence[1] = mutable_buffer(data.data() + offset, MAX_PAYLOAD);
    const size_t count = socket_.receive_from(bufferSequence, senderEndpoint, FLAGS, error);
    if (error)
    {
      auto message = boost::format("ERROR: failed to read byte count from data socket: %i: %s") % error.value() % error.message();
      BOOST_THROW_EXCEPTION(RckamException(message.str()));
    }
    //RCKAM_THREAD_CERR << "INFO: received " << count << " bytes: " << *imageId << ":" << *packetId << ": expected " << currentImageId_ << ":" << nextPacketId << std::endl;
    // if first packet of the image, initialize size, offset, counter and flags
    if (0 == *packetId)
    {
      if (0 != nextPacketId)
      {
        RCKAM_THREAD_CERR << "WARNING: lost packets for image " << currentImageId_ << " discarding" << std::endl;
      }
      // check that the images are in sequence
      if (static_cast<uint16_t>(currentImageId_ + 1) != *imageId)
      {
        RCKAM_THREAD_CERR << "WARNING: unexpected image id: expected " << static_cast<uint16_t>(currentImageId_ + 1) << ": receiving " << (*imageId) << std::endl;
      }
      currentImageId_ = *imageId;
      // the payload should be sizeof(size_t)
      if (count != header.size() + sizeof(size_t))
      {
        RCKAM_THREAD_CERR << "WARNING: first packet for image " << currentImageId_ << " has incorrect size: expected " << (header.size() + sizeof(size_t)) << " bytes: received " << count << " bytes: discarding" << std::endl;
        nextPacketId = -1;
        discard = true;
        offset = 0;
        continue;
      }
      byteCount = *reinterpret_cast<size_t *>(data.data() + offset);
      offset = 0;
      // resize data buffer to next multiple of MAX_PAYLOAD to allow for lost packets
      const size_t resizeTo = (((byteCount + MAX_PAYLOAD - 1) / MAX_PAYLOAD) * MAX_PAYLOAD);
      //RCKAM_THREAD_CERR << "INFO: image byteCount = " << byteCount << " - resizing buffer to " << resizeTo << " bytes" << std::endl;
      data.resize(resizeTo);
      nextPacketId = 1;
      discard = false;
      continue;
    }
    // nextPacketId is > 0
    if (discard || (*imageId != currentImageId_) || (*packetId != nextPacketId))
    {
      // we lost the end of currentImageId_ and the beginning of (*imageId) - discard both
      discard = true;
      currentImageId_ = *imageId;
      nextPacketId = -1;
      offset = 0;
      continue;
    }
    // we received the expected packet of data
    assert(byteCount > offset);
    const size_t remaining = byteCount - offset;
    const size_t payloadSize = std::min(MAX_PAYLOAD, remaining);
    const size_t expectedSize = payloadSize + header.size();
    if (count != expectedSize)
    {
      RCKAM_THREAD_CERR << "WARNING: packet " << *packetId << " for image " << currentImageId_ << " has incorrect size: expected " << expectedSize << ": received " << count << std::endl;
      discard = true;
      nextPacketId = -1;
      continue;
    }
    offset += payloadSize;
    done = (byteCount == offset);
    ++nextPacketId;
  }
  // erase the extra space
  data.resize(byteCount);
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

void ImageLoader::start(ImagePreview &imagePreview, const std::string &ipAddress, unsigned dataPort)
{
  boost::asio::ip::udp::endpoint remoteEndpoint(boost::asio::ip::make_address_v4(ipAddress), dataPort);
  constexpr boost::asio::socket_base::message_flags FLAGS = 0;
  assert(!isRunning());
  RCKAM_THREAD_CERR << "INFO: starting image loader..." << std::endl;
  using namespace boost::asio;
  using ip::udp;
  using common::RckamException;
  boost::system::error_code error;
  RCKAM_THREAD_CERR << "INFO: connecting image loader to " <<  ipAddress << ":" << dataPort << "..." << std::endl;
  // TCP
  //socket_.connect( udp::endpoint( ip::make_address(ipAddress), dataPort), error);
  std::array<char, 1> sendBuffer{0};
  socket_.open(udp::v4());
  socket_.send_to(boost::asio::buffer(sendBuffer), remoteEndpoint, FLAGS, error);
  if (error)
  {
    //auto message = boost::format("ERROR: failed to connect to data socket '%s:%d' (check that server is ready): %d: %s") % ipAddress % dataPort % error.value() % error.message();
    auto message = boost::format("ERROR: failed to send to remote socket '%s:%d' (check that server is ready): %d: %s") % ipAddress % dataPort % error.value() % error.message();
    BOOST_THROW_EXCEPTION(RckamException(message.str()));
  }
  else
  {
    RCKAM_THREAD_CERR << "INFO: connecting image loader to " <<  ipAddress << ":" << dataPort << "... done" << std::endl;
    RCKAM_THREAD_CERR << "INFO: starting threads..." << std::endl;
    std::unique_lock lock(mutex_);
    stop_ = false;
    for (auto &bufferEmpty: buffersEmpty_)
    {
      bufferEmpty = true;
    }
    imagePreview_ = &imagePreview;
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
    //RCKAM_THREAD_CERR << "INFO: acquiring empty buffer      " << std::setw(5) << i << "..." << std::endl;
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
    //RCKAM_THREAD_CERR << "INFO: acquiring full buffer  " << std::setw(5) << i << "..." << std::endl;
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

