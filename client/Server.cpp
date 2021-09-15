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

#include "client/Server.hpp"

#include <boost/asio.hpp>

namespace rckam
{
namespace client
{

Server::Server(const RckamOptions &options)
: ipAddress_(options.ipAddress)
, dataPort_(options.dataPort)
, controlPort_(options.controlPort)
{
}

Server::~Server()
{
}

const std::string &Server::ipAddress() const
{
  return ipAddress_;
}

void Server::setIpAddress(const std::string &ipAddress)
{
  ipAddress_ = ipAddress;
}

unsigned Server::dataPort() const
{
  return dataPort_;
}

void Server::setDataPort(unsigned dataPort)
{
  dataPort_ = dataPort;
}

unsigned Server::controlPort() const
{
  return controlPort_;
}

void Server::setControlPort(unsigned controlPort)
{
  controlPort_ = controlPort;
}

bool Server::isReady()
{
  return false;
}
  
std::vector<CameraInfo> Server::listCameras(boost::system::error_code error)
{
  std::vector<CameraInfo> cameraList;
  const std::string response = query(common::ServerCommand::LIST_CAMERAS, error);
  if (error)
  {
    return cameraList;
  }
  std::istringstream is(response);
  while(is)
  {
    CameraInfo cameraInfo;
    getline(is, cameraInfo.model, '\t');
    getline(is, cameraInfo.port);
    cameraList.emplace_back(cameraInfo);
  }
  return cameraList;
}

std::string Server::query(const common::ServerCommand command, boost::system::error_code error)
{
  using namespace boost::asio;
  using ip::tcp;
  boost::asio::io_service io_service;
  tcp::socket socket(io_service);
  socket.connect( tcp::endpoint( boost::asio::ip::make_address(ipAddress_), controlPort_));
  boost::asio::write( socket, boost::asio::const_buffer(reinterpret_cast<const char *>(command), sizeof(command)), error );
  if (error)
  {
    return std::string();
  }
  boost::asio::streambuf buffer;
  boost::asio::read(socket, buffer, boost::asio::transfer_all(), error);
  if(error && error != boost::asio::error::eof)
  {
    return std::string();
  }
  else
  {
    std::istream is(&buffer);
    std::string s;
    is >> s;
    return s;
  }
}

} // namespace client
} // namespace rckam

