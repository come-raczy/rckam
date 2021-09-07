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

#include "Debug.hpp"
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
  for (unsigned int i = 0; 100 > i; ++i)
  {
    CameraFile cameraFile;
    common::LockedOstream(std::cerr) << "capturing " << std::setw(3) << "..." << std::endl;
    const size_t byteCount = camera_.capturePreview(cameraFile);
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
    const auto dataAndSize = cameraFile.getDataAndSize();
    const auto count2 = boost::asio::write( socket_, boost::asio::buffer(std::get<0>(dataAndSize), byteCount), boost::asio::transfer_exactly(byteCount), error);
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
  }
}

} // namespace camera
} // namespace rckam

