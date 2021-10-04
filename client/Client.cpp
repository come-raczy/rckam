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

#include "client/Client.hpp"

#include <boost/asio.hpp>

namespace rckam
{
namespace client
{

Client::Client(const RckamClientOptions &options)
: ipAddress_(options.ipAddress)
, dataPort_(options.dataPort)
, controlPort_(options.controlPort)
{
}

Client::~Client()
{
}

const std::string &Client::ipAddress() const
{
  return ipAddress_;
}

void Client::setIpAddress(const std::string &ipAddress)
{
  ipAddress_ = ipAddress;
}

unsigned Client::dataPort() const
{
  return dataPort_;
}

void Client::setDataPort(unsigned dataPort)
{
  dataPort_ = dataPort;
}

unsigned Client::controlPort() const
{
  return controlPort_;
}

void Client::setControlPort(unsigned controlPort)
{
  controlPort_ = controlPort;
}

bool Client::isReady()
{
  return false;
}
  
std::vector<CameraInfo> Client::listCameras(boost::system::error_code error)
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

std::string Client::query(const common::ServerCommand command, boost::system::error_code error)
{
  using namespace boost::asio;
  using ip::tcp;
  boost::asio::io_service io_service;
  tcp::socket socket(io_service);
  RCKAM_THREAD_CERR << "INFO: connecting socket to " << ipAddress_ << ":" << controlPort_ << std::endl;
  socket.connect( tcp::endpoint( boost::asio::ip::make_address(ipAddress_), controlPort_), error);
  if (error)
  {
    RCKAM_THREAD_CERR << "ERROR: failed to connect socket: " << error.value() << ": " << error.message() << std::endl;
    return std::string();
  }
  boost::asio::const_buffer cbuffer(reinterpret_cast<const char *>(&command), sizeof(command));
  RCKAM_THREAD_CERR << "INFO: sending " << sizeof(command) << " bytes:";
  for (unsigned int i=0; sizeof(command) > i; ++i)
  {
    std::cerr << " " << (int)(*(reinterpret_cast<const char *>(cbuffer.data())+i));
  }
  std::cerr << std::endl;
  //const size_t count  = boost::asio::write( socket, boost::asio::const_buffer(reinterpret_cast<const char *>(&command), sizeof(command)), error);
  //const size_t count  = boost::asio::write( socket, cbuffer, boost::asio::transfer_all(), error);
  const size_t count  = boost::asio::write( socket, cbuffer, boost::asio::transfer_all(), error);
  if (error)
  {
    RCKAM_THREAD_CERR << "ERROR: failed to send command to the server: " << error.value() << ": " << error.message() << std::endl;
    return std::string();
  }
  if (sizeof(command) != count)
  {
    RCKAM_THREAD_CERR << "ERROR: failed to send " << sizeof(command) << " bytes to the server: " << count << " bytes sent" << std::endl;
    return std::string();
  }
  RCKAM_THREAD_CERR << "INFO: successfully sent " << count << " bytes" << std::endl;
#if 0
  // TODO: fix the issue with buffer
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
#endif
  return std::string();
}

} // namespace client
} // namespace rckam

