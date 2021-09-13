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
, io_service_()
, acceptor_(io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), dataPort))
, socket_(io_service_)
, communicationSocket_()
{
}

CameraController::~CameraController()
{
}

void CameraController::run()
{
  // wait for the connection
  acceptor_.accept(socket_);
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
    common::LockedOstream(std::cerr) << "capturing " << std::setw(4) << i << "..." << std::endl;
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
    common::LockedOstream(std::cerr) << "capturing " << std::setw(4) << i << "... done (" << byteCount << " bytes)" << std::endl;
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
    common::LockedOstream(std::cerr) << "sending " << std::setw(4) << i << "... (" << byteCount << " bytes)" << std::endl;
    // const std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    // common::LockedOstream(std::cerr) << "capture completed. Received " << size << " bytes" << std::endl;
    // auto in_time_t = std::chrono::system_clock::to_time_t(now);
    // const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;
    // const auto timeString = std::put_time(std::localtime(&in_time_t), "%X");
    // auto fileName = boost::format("/tmp/preview-%05i-%s.%03i.jpg") % i % timeString % ms;
    // std::ofstream os(fileName.str());
    // os << cameraFile;
    boost::system::error_code error;
    const auto count1 = boost::asio::write( socket_, boost::asio::buffer(&byteCount, sizeof(size_t)), boost::asio::transfer_exactly(sizeof(size_t)), error);
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
    const auto count2 = boost::asio::write( socket_, boost::asio::buffer(data, byteCount), boost::asio::transfer_exactly(byteCount), error);
    if (error)
    {
      auto message = boost::format("ERROR: failed to send image data to data socket: %i: %s") % error.value() % error.message();
      BOOST_THROW_EXCEPTION(RckamException(message.str()));
    }
    else if (byteCount != count2)
    {
      auto message = boost::format("ERROR: failed to send %i bytes of image data to data socket: read only %i") % byteCount % count2;
      BOOST_THROW_EXCEPTION(RckamException(message.str()));
    }
    common::LockedOstream(std::cerr) << "sending " << std::setw(4) << i << "... done" << std::endl;
    cameraFilesEmpty_[index] = true;
    conditionVariables_[index].notify_one();
    index = (index + 1) % 2;
    ++i;
  }
}

} // namespace camera
} // namespace rckam

