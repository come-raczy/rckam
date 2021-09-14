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

#include "CameraController.hpp"

#include <iostream>
#include <fstream>
#include <boost/format.hpp>
#include <chrono>
#include <iomanip>

#include "common/Debug.hpp"
#include "Exceptions.hpp"

namespace rckam
{
namespace camera
{

CameraController::CameraController(const char * const model, const char * const usbPort, const unsigned dataPort, Gphoto2Context &context)
: camera_(model, usbPort, context)
//, dataSocket_()
//, io_service_()
//, acceptor_(io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), dataPort))
//, socket_(io_service_)
//, resolver_(io_context_)
, io_context_()
, socket_(io_context_, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), dataPort))
//, communicationSocket_()
{
}

CameraController::~CameraController()
{
}

void CameraController::run()
{
  // wait for the connection - TCP
  // acceptor_.accept(socket_);
  // wait for the connection - UDP
  std::array<char, 1> receiveBuffer;
  boost::system::error_code error;
  constexpr boost::asio::socket_base::message_flags FLAGS = 0;
  RCKAM_THREAD_CERR << "INFO: waiting for data on " << socket_.local_endpoint().address() << ":" << socket_.local_endpoint().port() << "..." << std::endl;
  socket_.receive_from(boost::asio::buffer(receiveBuffer), remoteEndpoint_, FLAGS, error);
  if (error)
  {
    RCKAM_THREAD_CERR << "ERROR: socket failed to receive from remote: " << error.value() << ": " << error.message() << std::endl;
    BOOST_THROW_EXCEPTION(RckamException(std::string("socket failed to receive from remote: ") +  error.message()));
  }
  RCKAM_THREAD_CERR << "INFO: sending image previews to " << remoteEndpoint_.address() << ":" << remoteEndpoint_.port() << std::endl;
  // create a thread to send the data
  stopPreview_ = false;
  transferDataException_ = nullptr;
  std::thread dataTransferThread = std::thread(&CameraController::transferDataWrapper, this);;
  for (auto &cameraFileEmpty: cameraFilesEmpty_)
  {
    cameraFileEmpty = true;
  }
  unsigned index = 0;
  unsigned i = 0;
  while (!stopPreview_)
  {
    std::unique_lock<std::mutex> lock(mutexes_[index]);
    conditionVariables_[index].wait(lock, [&] {return cameraFilesEmpty_[index] || stopPreview_;});
    if (stopPreview_)
    {
      break;
    }
    CameraFile &cameraFile = cameraFiles_[index];
    RCKAM_THREAD_CERR << "INFO: capturing " << std::setw(4) << i << "..." << std::endl;
    const std::chrono::time_point<std::chrono::system_clock> captureBegin = std::chrono::system_clock::now();
    const size_t byteCount = camera_.capturePreview(cameraFile);
    const std::chrono::time_point<std::chrono::system_clock> captureEnd = std::chrono::system_clock::now();
    if (0 == (i % 100))
    {
      auto in_time_t = std::chrono::system_clock::to_time_t(captureBegin);
      const auto beginMs = std::chrono::duration_cast<std::chrono::milliseconds>(captureBegin.time_since_epoch()).count() % 1000;
      const auto durationMs = std::chrono::duration_cast<std::chrono::milliseconds>(captureEnd.time_since_epoch()).count() 
                            - std::chrono::duration_cast<std::chrono::milliseconds>(captureBegin.time_since_epoch()).count();
      const auto timeString = std::put_time(std::localtime(&in_time_t), "%X");
      auto fileName = boost::format("/tmp/preview-%05i-%s.%03i-%05i.jpg") % i % timeString % beginMs % durationMs;
      std::ofstream os(fileName.str());
      os << cameraFile;
    }
    RCKAM_THREAD_CERR << "INFO: capturing " << std::setw(4) << i << "... done (" << byteCount << " bytes)" << std::endl;
    cameraFilesEmpty_[index] = false;
    conditionVariables_[index].notify_one();
    index = (index + 1) % 2;
    ++i;
  }
  stopPreview_ = true;
  for (auto &conditionVariable: conditionVariables_)
  {
    conditionVariable.notify_all();
  }
  if (dataTransferThread.joinable())
  {
    dataTransferThread.join();
  }
}

void CameraController::transferDataWrapper()
{
  try
  {
    transferData();
  }
  catch(std::exception &e)
  {
    RCKAM_THREAD_CERR << "WARNING: data transfer thread terminated: " << e.what() << std::endl;
    transferDataException_ = std::current_exception();
  }
  catch(...)
  {
    RCKAM_THREAD_CERR << "WARNING: data transfer thread terminated: unkmown exception" << std::endl;
    transferDataException_ = std::current_exception();
  }
}

void CameraController::transferData()
{
  constexpr boost::asio::socket_base::message_flags FLAGS = 0;
  unsigned index = 0;
  unsigned i = 0;
  while (!stopPreview_)
  {
    std::unique_lock<std::mutex> lock(mutexes_[index]);
    conditionVariables_[index].wait(lock, [&]{return (false == cameraFilesEmpty_[index]) || stopPreview_;});
    if (stopPreview_)
    {
      break;
    }
    CameraFile &cameraFile = cameraFiles_[index];
    const auto dataAndSize = cameraFile.getDataAndSize();
    const auto data = std::get<0>(dataAndSize);
    const auto byteCount = std::get<1>(dataAndSize);
    RCKAM_THREAD_CERR << "INFO: sending " << std::setw(4) << i << "... (" << byteCount << " bytes)" << std::endl;
    // const std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    // common::LockedOstream(std::cerr) << "capture completed. Received " << size << " bytes" << std::endl;
    // auto in_time_t = std::chrono::system_clock::to_time_t(now);
    // const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;
    // const auto timeString = std::put_time(std::localtime(&in_time_t), "%X");
    // auto fileName = boost::format("/tmp/preview-%05i-%s.%03i.jpg") % i % timeString % ms;
    // std::ofstream os(fileName.str());
    // os << cameraFile;
    boost::system::error_code error;
    // TCP
    //const auto count1 = boost::asio::write( socket_, boost::asio::buffer(&byteCount, sizeof(size_t)), boost::asio::transfer_exactly(sizeof(size_t)), error);
    // UDP
    const auto count1 = socket_.send_to(boost::asio::buffer(&byteCount, sizeof(size_t)), remoteEndpoint_, FLAGS, error); 
    if (error)
    {
      auto message = boost::format("ERROR: failed to send byte count to data socket: %i: %s") % error.value() % error.message();
      BOOST_THROW_EXCEPTION(RckamException(message.str()));
    }
    else if (sizeof(size_t) != count1)
    {
      auto message = boost::format("ERROR: failed to send %i bytes for byte count to data socket: read only %i") % sizeof(size_t) % count1;
      BOOST_THROW_EXCEPTION(RckamException(message.str()));
    }
    // TCP
    //const auto count2 = boost::asio::write( socket_, boost::asio::buffer(data, byteCount), boost::asio::transfer_exactly(byteCount), error);
    // UDP
    size_t offset = 0;
    while(byteCount > offset)
    {
      constexpr size_t MAX_PACKET_SIZE = 1472;
      const size_t remaining = byteCount - offset;
      const size_t size = std::min(MAX_PACKET_SIZE, remaining);
      const auto count2 = socket_.send_to(boost::asio::buffer(data, byteCount), remoteEndpoint_, FLAGS, error); 
      if (error)
      {
        auto message = boost::format("ERROR: failed to send image data to data socket: %i: %s") % error.value() % error.message();
        BOOST_THROW_EXCEPTION(RckamException(message.str()));
      }
      else if (size != count2)
      {
        auto message = boost::format("ERROR: failed to send %i bytes of image data to data socket: read only %i") % size % count2;
        BOOST_THROW_EXCEPTION(RckamException(message.str()));
      }
      offset += size;
    }
    RCKAM_THREAD_CERR << "INFO: sending " << std::setw(4) << i << "... done" << std::endl;
    cameraFilesEmpty_[index] = true;
    conditionVariables_[index].notify_one();
    index = (index + 1) % 2;
    ++i;
  }
}

} // namespace camera
} // namespace rckam

