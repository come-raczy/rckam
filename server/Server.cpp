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
  io_service ioService;
  tcp::acceptor acceptor(ioService, tcp::endpoint(tcp::v4(), controlPort_));
  tcp::socket socket(ioService);
  stop_ = false;
  while (!stop_)
  {
    boost::system::error_code error;
    // wait for a request
    acceptor.accept(socket);
    boost::asio::streambuf buffer;
    const unsigned count = boost::asio::read(socket, buffer, boost::asio::transfer_all(), error);
    if(error)
    {
      RCKAM_THREAD_CERR << "WARNING: error while reading the control socket: " << error.value() << error.message() << std::endl;
      continue;
    }
    if (sizeof(ServerCommand) > count)
    //read operation
    {
      RCKAM_THREAD_CERR << "WARNING: error while reading the control socket: message too short: received " << count << " bytes - ServerCommands is " << sizeof(ServerCommand) << " bytes" << std::endl;
      continue;
    }
    std::string data(boost::asio::buffer_cast<const char*>(buffer.data()));
    assert(data.size() > sizeof(ServerCommand));
    const ServerCommand command = *(reinterpret_cast<ServerCommand *>(data.data()));
    switch (command)
    {
      case ServerCommand::ECHO_ONLY:
        sendResponse(socket, std::move(data));
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

