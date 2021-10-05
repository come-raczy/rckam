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

#include "Server.hpp"

#include <array>
#include <string>
#include <boost/asio.hpp>

#include "common/Rckam.hpp"
#include "common/Debug.hpp"
#include "common/Exceptions.hpp"

namespace rckam
{
namespace server
{

Server::Server(const unsigned controlPort, const unsigned dataPort)
: controlPort_(controlPort)
, stop_(true)
, gphoto2Context_()
, cameraController_(dataPort, gphoto2Context_)
{
}

Server::~Server()
{
}

void Server::run()
{
  using common::ServerCommand;
  using namespace boost::asio;
  using ip::tcp;
  RCKAM_THREAD_CERR << "INFO: listening for control requests on port " << controlPort_ << std::endl;
  io_service ioService;
  tcp::acceptor acceptor(ioService, tcp::endpoint(tcp::v4(), controlPort_));
  stop_ = false;
  std::array<char, common::COMMAND_BUFFER_SIZE> buffer;
  while (!stop_)
  {
    boost::system::error_code error;
    RCKAM_THREAD_CERR << "INFO: accepting requests..." << std::endl;
    tcp::socket socket(ioService);
    acceptor.accept(socket);
    RCKAM_THREAD_CERR << "INFO: reading data" << std::endl;
    // must specify the size of the data otherwise the read will block until the client closes the connection
    const auto count = boost::asio::read(socket, mutable_buffer(buffer.data(), buffer.size()), boost::asio::transfer_exactly(sizeof(ServerCommand)), error);
    if(error && (boost::asio::error::eof != error.value()))
    {
      RCKAM_THREAD_CERR << "WARNING: error while reading the control socket: " << error.value() << ": " << error.message() << std::endl;
      continue;
    }
    if (sizeof(ServerCommand) > count)
    {
      RCKAM_THREAD_CERR << "WARNING: error while reading the control socket: message too short: received " << count << " bytes - ServerCommands is " << sizeof(ServerCommand) << " bytes" << std::endl;
      continue;
    }
    RCKAM_THREAD_CERR << "INFO: succesfully read " << count << " bytes of data" << std::endl;
    const auto available = socket.available();
    if (available)
    {
      RCKAM_THREAD_CERR << "INFO: reading " << available << " additional bytes of data" << std::endl;
      const auto count = boost::asio::read(socket, mutable_buffer(buffer.data(), buffer.size()), boost::asio::transfer_exactly(available), error);
      if((error && (boost::asio::error::eof != error.value())) || (available != count))
      {
        RCKAM_THREAD_CERR << "WARNING: error while reading " << available << " bytes of additional data on the control socket: read " << count << " bytes: " << error.value() << ": " << error.message() << std::endl;
        continue;
      }
    }
    const ServerCommand command = *(reinterpret_cast<ServerCommand *>(buffer.data()));
    switch (command)
    {
      case ServerCommand::ECHO_ONLY:
        sendResponse(socket, std::string(buffer.data(), count + available));
        break;
      case ServerCommand::LIST_CAMERAS:
        sendResponse(socket, cameraController_.listCameras());
        break;
      default:
        sendResponse(socket, commandNotSupported());
    }
  }
}

void Server::sendResponse(boost::asio::ip::tcp::socket &socket, const std::string &&data)
{
  boost::system::error_code error;
  RCKAM_THREAD_CERR << "INFO: sendResponse: " << data.size() << " bytes: " << data << std::endl;
  boost::asio::write(socket, boost::asio::buffer(data), error);
  if(error)
  {
    RCKAM_THREAD_CERR << "WARNING: error while sending response on control socket: " << error.value() << ": " << error.message() << std::endl;
  }
}

std::string Server::commandNotSupported()
{
  static const std::string response{0,common::ResponseCode::NOT_SUPPORTED};
  return response;
}

} // namespace server
} // namespace rckam

